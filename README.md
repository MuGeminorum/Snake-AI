# PPO-Snake-AI

[![Python application](https://github.com/george-chou/PPO-Snake-AI/actions/workflows/python-app.yml/badge.svg?branch=main)](https://github.com/george-chou/PPO-Snake-AI/actions/workflows/python-app.yml)
[![license](https://img.shields.io/github/license/george-chou/PPO-Snake-AI.svg)](https://github.com/george-chou/PPO-Snake-AI/blob/master/LICENSE)

This project aims to use deep reinforcement learning (DRL) to play Snake game automatically. The core DRL method used here is PPO for discrete, which has brilliant performance in the field of discrete action space like in continuous action space. You just need half an hour to train the snake agent and then it can take effect.

## Requirements
```
conda create -n ppo --yes --file conda.txt
conda activate ppo
pip install -r requirements.txt
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

## Experiments
| Round        |                                                        1                                                         |                                                        2                                                         |                                                        3                                                         |
| :----------- | :--------------------------------------------------------------------------------------------------------------: | :--------------------------------------------------------------------------------------------------------------: | :--------------------------------------------------------------------------------------------------------------: |
| Traing curve | ![round1](https://user-images.githubusercontent.com/20459298/233120722-d300c250-a07e-44c1-8986-d1f26d48c0f8.png) | ![round2](https://user-images.githubusercontent.com/20459298/233120780-43c9b35b-def6-4a57-b7b4-6599ad594c5c.png) | ![round3](https://user-images.githubusercontent.com/20459298/233120831-deb18303-25ec-4ff8-bafc-4726d1a81af4.png) |
| Evaluation   | ![round1](https://user-images.githubusercontent.com/20459298/233120884-b0ea6080-8aa4-4382-9ce5-90c22737cdf3.gif) | ![round2](https://user-images.githubusercontent.com/20459298/233121028-f9431608-3833-49d5-9cde-573fdb82c692.gif) | ![round3](https://user-images.githubusercontent.com/20459298/233121080-9a4f2e95-0f49-40cf-91a4-f7f57d4b861f.gif) |
| Reward_eat   |                                                       +2.0                                                       |                                                       +2.0                                                       |                                                       +2.0                                                       |
| Reward_hit   |                                                       -0.5                                                       |                                                       -1.0                                                       |                                                       -1.5                                                       |
| Reward_bit   |                                                       -0.8                                                       |                                                       -1.5                                                       |                                                       -2.0                                                       |
| Avg record   |                                                       ≈19                                                        |                                                       ≈23                                                        |                                                       ≈28                                                        |

## Conclusion
1. Increasing the penalty for death leads to higher average records
2. The training result of the low death penalty strategy has a low reward curve, but it performs well in the demo
3. A particularly high reward for eating food can lead to quick success regardless of long-term safety

## Future work
1. Training time is too short to reflect the advantages of DRL compared to none-DRL method ([Snaqe](https://github.com/george-chou/Snaqe))
2. The zigzag of snake body looks ugly, try to add punishment into reward for too many zigzags
