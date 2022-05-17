import random
import argparse
from snake import *


parser = argparse.ArgumentParser(description='test')
parser.add_argument('--weight', type=str,
                    default='./model/act-weight_round3_472_82.5.pkl', help='Select saved model.')
args = parser.parse_args()

if __name__ == "__main__":
    random.seed(100)
    env = Snake()
    env.snake_speed = 20
    agent = AgentDiscretePPO()
    agent.init(512, 6, 4)
    agent.act.load_state_dict(torch.load(args.weight))
    for _ in range(15):
        o = env.reset()
        while 1:
            env.render()
            for event in pygame.event.get():  # If you don't add this render, it will freeze
                pass
            a, _ = agent.select_action(o)
            o2, r, d, _ = env.step(a)
            o = o2
            if d:
                break
