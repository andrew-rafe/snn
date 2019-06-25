import gym
import numpy as np

def write_obs(i, obs):
    obs = obs.flatten()
    obs = obs / 255;

    file_name = "tmp/obs_" + str(i)
    f = open(file_name, "w")
    for i in range(obs.size):
        f.write("%f\n" % obs[i])

def main():
    env = gym.make("SpaceInvaders-v0")
    observation = env.reset()
    
    print(env.action_space.n)
    for i in range(1000):
        write_obs(i, observation)
        env.render()
        observation, reward, done, info = env.step(env.action_space.sample())
        if (done):
            break
            
    env.close()




if __name__ == "__main__":
    main()