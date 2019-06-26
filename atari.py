import gym
import time
from pyNet import PyNetwork
import numpy as np

# Create network as global variable
network = PyNetwork(2, {4, 2})

def rgb2gray(rgb):
    return np.dot(rgb[...,:3], [0.2989, 0.5870, 0.1140])

def get_out(timestep, obs):
    #obs = rgb2gray(obs)
    obs = np.asarray(obs)
    obs.flatten()
    #obs = obs / 255;
    num_inputs = obs.size
    return network.process_inputs(obs, timestep+1, num_inputs)

def main():
    env = gym.make("CartPole-v1")
    observation = env.reset()
    print(observation);
    for i in range(1000):
        get_out(i, observation)
        print(network.get_outputs())
        env.render()
        observation, reward, done, info = env.step(env.action_space.sample())
        if (done):
            observation = env.reset()

    env.close()

if __name__ == "__main__":
    main()
