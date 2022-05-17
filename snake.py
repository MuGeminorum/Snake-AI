import os
import random
import pygame
import sys
from pygame.locals import *
import numpy as np
from Agent import AgentDiscretePPO
import torch
from PIL import Image
from torchvision import transforms

round = 1
rewards = []

rewards.append({
    'eat': 2.0,
    'hit': -0.5,
    'bit': -0.8
})

rewards.append({
    'eat': 2.0,
    'hit': -1.0,
    'bit': -1.5
})

rewards.append({
    'eat': 2.0,
    'hit': -1.5,
    'bit': -2.0
})


class Snake:
    def __init__(self):
        self.snake_speed = 100  # Snake speed
        self.windows_width = 600
        self.windows_height = 600  # the size of the game window
        self.cell_size = 50  # Snake body square size, note that the body size must be divisible by the length and width of the window
        self.map_width = int(self.windows_width / self.cell_size)
        self.map_height = int(self.windows_height / self.cell_size)
        self.white = (255, 255, 255)
        self.black = (0, 0, 0)
        self.gray = (230, 230, 230)
        self.dark_gray = (40, 40, 40)
        self.DARKGreen = (0, 155, 0)
        self.Green = (0, 255, 0)
        self.Red = (255, 0, 0)
        self.blue = (0, 0, 255)
        self.dark_blue = (0, 0, 139)
        self.cyan = (0, 255, 255)
        self.yellow = (255, 255, 0)
        self.BG_COLOR = self.black  # game background color

        # define direction
        self.UP = 0
        self.DOWN = 1
        self.LEFT = 2
        self.RIGHT = 3
        self.HEAD = 0  # Snake head subscript

        pygame.init()  # Module initialization
        self.snake_speed_clock = pygame.time.Clock()  # Create a Pygame clock object

        [self.snake_coords, self.direction, self.food,
            self.state] = [None, None, None, None]

    def reset(self):
        startx = random.randint(3, self.map_width - 8)  # start position
        starty = random.randint(3, self.map_height - 8)
        self.snake_coords = [{'x': startx, 'y': starty},  # Initial Snake
                             {'x': startx - 1, 'y': starty},
                             {'x': startx - 2, 'y': starty}]
        self.direction = self.RIGHT  # move right at start
        self.food = self.get_random_location()  # food random location
        return self.getState()

    def step(self, action):
        if action == self.LEFT and self.direction != self.RIGHT:
            self.direction = self.LEFT
        elif action == self.RIGHT and self.direction != self.LEFT:
            self.direction = self.RIGHT
        elif action == self.UP and self.direction != self.DOWN:
            self.direction = self.UP
        elif action == self.DOWN and self.direction != self.UP:
            self.direction = self.DOWN
        self.move_snake(self.direction, self.snake_coords)
        ret = self.snake_is_alive(self.snake_coords)
        d = (ret > 0)
        flag = self.snake_is_eat_food(self.snake_coords, self.food)
        reward = self.getReward(flag, ret)

        return [self.getState(), reward, d, None]

    def getReward(self, flag, ret):
        reward = 0
        if flag:
            reward += rewards[round - 1]['eat']
        # [xhead,yhead] = [self.snake_coords[self.HEAD]['x'],self.snake_coords[self.HEAD]['y']]
        # [xfood,yfood] = [self.food['x'],self.food['y']]
        # distance1 = np.sqrt((xhead-xfood)**2+(yhead-yfood)**2)
        # if distance1 < 1:
        #     reward += (1-distance1)/1
        if ret == 1:
            reward += rewards[round - 1]['hit']

        if ret == 2:
            reward += rewards[round - 1]['bit']

        return reward

    def render(self):
        self.screen = pygame.display.set_mode(
            (self.windows_width, self.windows_height))
        self.screen.fill(self.BG_COLOR)
        self.draw_snake(self.screen, self.snake_coords)
        self.draw_food(self.screen, self.food)
        self.draw_score(self.screen, len(self.snake_coords)-3)
        pygame.display.update()
        self.snake_speed_clock.tick(self.snake_speed)  # control fps

    def getState(self):
        # Fundamentals 6 Dimensions
        [xhead, yhead] = [self.snake_coords[self.HEAD]
                          ['x'], self.snake_coords[self.HEAD]['y']]
        [xfood, yfood] = [self.food['x'], self.food['y']]
        deltax = (xfood - xhead) / self.map_width
        deltay = (yfood - yhead) / self.map_height
        checkPoint = [[xhead, yhead-1], [xhead-1, yhead],
                      [xhead, yhead+1], [xhead+1, yhead]]
        tem = [0, 0, 0, 0]
        for coord in self.snake_coords[1:]:
            if [coord['x'], coord['y']] in checkPoint:
                index = checkPoint.index([coord['x'], coord['y']])
                tem[index] = 1
        for i, point in enumerate(checkPoint):
            if point[0] >= self.map_width or point[0] < 0 or \
                    point[1] >= self.map_height or point[1] < 0:
                tem[i] = 1
        state = [deltax, deltay]
        state.extend(tem)

        # Add the position information of the middle and tail of the snake body, add 4 dimensions
        # length = len(self.snake_coords)
        # snake_mid = [self.snake_coords[int(length/2)]['x']-xhead,self.snake_coords[int(length/2)]['y']-yhead]
        # snake_tail = [self.snake_coords[-1]['x']-xhead,self.snake_coords[-1]['y']-yhead]
        # state.extend(snake_mid+snake_tail)
        return state

    def draw_food(self, screen, food):
        x = food['x'] * self.cell_size
        y = food['y'] * self.cell_size
        appleRect = pygame.Rect(x, y, self.cell_size, self.cell_size)
        pygame.draw.rect(screen, self.Red, appleRect)

    # Draw the greedy snake
    def draw_snake(self, screen, snake_coords):
        for i, coord in enumerate(snake_coords):
            color = self.Green
            if i == 0:
                color = self.yellow

            if i == len(snake_coords) - 1:
                color = self.cyan

            x = coord['x'] * self.cell_size
            y = coord['y'] * self.cell_size
            wormSegmentRect = pygame.Rect(x, y, self.cell_size, self.cell_size)
            pygame.draw.rect(screen, self.dark_blue, wormSegmentRect)
            wormInnerSegmentRect = pygame.Rect(  # The second layer of bright green inside the snake's body
                x + 4, y + 4, self.cell_size - 8, self.cell_size - 8)
            pygame.draw.rect(screen, color, wormInnerSegmentRect)

    # mobile snake
    def move_snake(self, direction, snake_coords):
        if direction == self.UP:
            newHead = {'x': snake_coords[self.HEAD]['x'],
                       'y': snake_coords[self.HEAD]['y'] - 1}
        elif direction == self.DOWN:
            newHead = {'x': snake_coords[self.HEAD]['x'],
                       'y': snake_coords[self.HEAD]['y'] + 1}
        elif direction == self.LEFT:
            newHead = {'x': snake_coords[self.HEAD]
                       ['x'] - 1, 'y': snake_coords[self.HEAD]['y']}
        elif direction == self.RIGHT:
            newHead = {'x': snake_coords[self.HEAD]
                       ['x'] + 1, 'y': snake_coords[self.HEAD]['y']}
        else:
            newHead = None
            raise Exception('error for direction!')

        snake_coords.insert(0, newHead)

    # Determine if the snake is dead
    def snake_is_alive(self, snake_coords):
        tag = 0
        if snake_coords[self.HEAD]['x'] == -1 or \
            snake_coords[self.HEAD]['x'] == self.map_width or \
            snake_coords[self.HEAD]['y'] == -1 or \
                snake_coords[self.HEAD]['y'] == self.map_height:
            tag = 1  # Snake hits the wall
        for snake_body in snake_coords[1:]:
            if snake_body['x'] == snake_coords[self.HEAD]['x'] and \
                    snake_body['y'] == snake_coords[self.HEAD]['y']:
                tag = 2  # Snake touches its body
        return tag

    # To determine whether the greedy snake has eaten food,
    # if it is a list or a dictionary,
    # then modifying the content of the parameters in the function will affect the objects outside the function.
    def snake_is_eat_food(self, snake_coords, food):
        flag = False
        if snake_coords[self.HEAD]['x'] == food['x'] and \
                snake_coords[self.HEAD]['y'] == food['y']:
            while True:
                food['x'] = random.randint(0, self.map_width - 1)
                food['y'] = random.randint(
                    0, self.map_height - 1)  # food location reset
                tag = 0
                for coord in snake_coords:
                    if [coord['x'], coord['y']] == [food['x'], food['y']]:
                        tag = 1
                        break
                if tag == 1:
                    continue
                break
            flag = True
        else:
            # If no food is eaten, move forward, then delete one space at the end
            del snake_coords[-1]
        return flag

    # Food is randomly generated
    def get_random_location(self):
        return {'x': random.randint(0, self.map_width - 1), 'y': random.randint(0, self.map_height - 1)}

    # draw grades
    def draw_score(self, screen, score):
        font = pygame.font.SysFont('Arial', 28)
        scoreSurf = font.render('Score: %s' % score, True, self.white)
        scoreRect = scoreSurf.get_rect()
        scoreRect.topleft = (self.windows_width - 130, 0)
        screen.blit(scoreSurf, scoreRect)

    @staticmethod
    # program terminated
    def terminate():
        pygame.quit()
        sys.exit()

    def screenTensor(self):
        self.render()
        image_data = pygame.surfarray.array3d(
            pygame.display.get_surface()).transpose((1, 0, 2))
        img = Image.fromarray(image_data.astype(np.uint8))
        # img = img.convert('1')
        transform = transforms.Compose([
            transforms.Resize((50, 50)),  # Only PIL images can be cropped
            transforms.ToTensor(),
        ])
        img_tensor = transform(img)
        # new_img_PIL = transforms.ToPILImage()(img_tensor).convert('RGB')
        # new_img_PIL.show()  # Processed PIL image

        return img_tensor


def filter_pkl(lists):
    new_list = []
    count = len(lists)
    for i in range(count):
        tmp = lists[i]
        if tmp[-4:] == '.pkl':
            new_list.append(tmp)

    return new_list


def get_latest_weight(path):
    lists = os.listdir(path)
    lists = filter_pkl(lists)
    lists.sort(key=lambda x: os.path.getmtime((path+"\\"+x)))
    return path + '/' + lists[-1]


if __name__ == "__main__":
    random.seed(100)
    env = Snake()
    env.snake_speed = 10
    agent = AgentDiscretePPO()
    agent.init(512, 6, 4)
    latest_weight = get_latest_weight('./history/')
    agent.act.load_state_dict(torch.load(latest_weight))
    for _ in range(15):
        o = env.reset()
        while 1:
            env.render()
            for event in pygame.event.get():
                pass  # If you don't add this render, it will freeze
            a, _ = agent.select_action(o)
            o2, r, d, _ = env.step(a)
            o = o2
            if d:
                break
