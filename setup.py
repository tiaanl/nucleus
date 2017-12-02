import os
import subprocess


def main():
    root_path = os.path.dirname(__file__)

    cmd = ['cmake',
           '-G', 'Visual Studio 15 2017 Win64',
           root_path]

    subprocess.call(cmd)


if __name__ == '__main__':
    main()
