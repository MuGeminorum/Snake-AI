import argparse
from painter import Painter

parser = argparse.ArgumentParser(description='plot')
parser.add_argument('--history', type=str,
                    default='./logs/reward_round3_82.5.csv', help='Select training log.')
args = parser.parse_args()

if __name__ == "__main__":
    rwd_path = args.history
    painter = Painter(load_csv=True, load_dir=rwd_path)
    painter.setTitle('snake game reward')
    painter.setXlabel('episode')
    painter.setYlabel('reward')
    painter.drawFigure()
