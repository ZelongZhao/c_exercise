import string
import subprocess
import random

createDBcmd = ["./ex17", "test.dat", "c", "512", "512"]


def check_stderr(process):
    if process.stderr:
        print("Error output detected:")
        print(process.stderr)
        raise RuntimeError("Subprocess stderr is not empty, exiting...")


def generate_set_command_line(original_number):
    new_number = original_number + 1

    name_length = random.randint(5, 10)
    random_name = ''.join(random.choices(string.ascii_uppercase, k=name_length))
    random_email = f"{random_name.lower()}@example.com"

    return ["./ex17", "test.dat", "s", str(new_number), random_name, random_email]


def generate_get_command_line(original_number):
    new_number = original_number + 1
    return ["./ex17", "test.dat", "g", str(new_number)]


all_set_commands = []
all_get_commands = []

original_base_number = 0
for _ in range(5):
    cmd_line = generate_set_command_line(original_base_number)
    all_set_commands.append(cmd_line)
    original_base_number += 1

original_base_number = 0
for _ in range(5):
    cmd_line = generate_get_command_line(original_base_number)
    all_get_commands.append(cmd_line)
    original_base_number += 1

process = subprocess.run(createDBcmd,  text=True)
check_stderr(process)

for i in range(0,5):
    process = subprocess.run(all_set_commands[i], capture_output=True, check=True, text=True)
    check_stderr(process)

for i in range(0,5):
    process = subprocess.run(all_get_commands[i], capture_output=True, check=True, text=True)
    check_stderr(process)
    last_three = all_set_commands[i][-3:]
    output_words = process.stdout.split('\n')[0].split(' ')
    if output_words != last_three:
        print("error")



