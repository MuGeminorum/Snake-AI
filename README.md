# PPO-Snake-AI

[![Python application](https://github.com/george-chou/PPO-Snake-AI/actions/workflows/python-app.yml/badge.svg?branch=main)](https://github.com/george-chou/PPO-Snake-AI/actions/workflows/python-app.yml)
[![license](https://img.shields.io/github/license/george-chou/PPO-Snake-AI.svg)](https://github.com/george-chou/PPO-Snake-AI/blob/master/LICENSE)

Using deep reinforcement learning to play Snake game.\
The used algorithm is PPO for discrete! It has the brilliant performance in the field of discrete action space just like in continuous action space.\
You just need half an hour to train the snake and then it can be as smart as you.

## Requirements
```
conda create -n ppo-snake python=3.8
conda install pytorch==1.12.1 torchvision==0.13.1 torchaudio==0.12.1 cudatoolkit=11.3 -c pytorch
pip install pygame==2.1.2
pip install pandas==1.5.2
pip install seaborn==0.12.1
pip install matplotlib==3.6.2
```

## Usage
### Train
```
python train.py # after training, the training curve of current round will autometically show
python snake.py # evaluate latest saved model
```

### Evaluate assigned model
```
python evaluate.py --weight ./model/act-weight_round3_472_82.5.pkl
```

### Plot assigned reward log
```
python plotter.py --history ./logs/reward_round3_82.5.csv
```

## Experiment
| Round        |                                 1                                 |                                 2                                 |                                 3                                 |
| :----------- | :---------------------------------------------------------------: | :---------------------------------------------------------------: | :---------------------------------------------------------------: |
| Traing curve | <img src="https://picrepo.netlify.app/PPO-Snake-AI/round1.png" /> | <img src="https://picrepo.netlify.app/PPO-Snake-AI/round2.png" /> | <img src="https://picrepo.netlify.app/PPO-Snake-AI/round3.png" /> |
| Evaluation   | <img src="https://picrepo.netlify.app/PPO-Snake-AI/round1.gif" /> | <img src="https://picrepo.netlify.app/PPO-Snake-AI/round2.gif" /> | <img src="https://picrepo.netlify.app/PPO-Snake-AI/round3.gif" /> |
| Reward_eat   |                               +2.0                                |                               +2.0                                |                               +2.0                                |
| Reward_hit   |                               -0.5                                |                               -1.0                                |                               -1.5                                |
| Reward_bit   |                               -0.8                                |                               -1.5                                |                               -2.0                                |
| Avg record   |                                ≈19                                |                                ≈23                                |                                ≈28                                |

## Conclusion
1. Increasing the penalty for death leads to higher average records
2. The training result of the low death penalty strategy has a low reward curve, but it performs well in the demo
3. A particularly high reward for eating food can lead to quick success regardless of long-term safety

## Future work
1. Training time is too short to reflect the advantages of DRL
2. The zigzag of snake body looks ugly, try to add punishment into reward for too many zigzags
