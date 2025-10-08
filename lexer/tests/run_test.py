import os
import glob
import shutil
import subprocess
from pathlib import Path

DEBUG = True

BASE_RUN_DIRECTORY = "."
BASE_IMAGE_NAME = "d_flex_project"
TEST_RESULT_DIRECTORY = "result"
EXECUTABLE_TARGET = "./d_compiler"

ENDLESS_LOOP_COMMAND = ["tail", "-f", "/dev/null"]

def process_d_files(directory):
    # Формируем путь с расширением .go
    d_files_pattern = os.path.join(directory, '**', '*.d')

    # Ищем все файлы, которые соответствуют шаблону
    d_files = glob.glob(d_files_pattern, recursive=True)

    # Обрабатываем каждый найденный файл
    return d_files

def build_local_image(image_name=BASE_IMAGE_NAME):
    # Локальная сборка (если используем CMake, Makefile, или подобные системы)
    # Например, для CMake
    command = ["cmake", ".", "-DCMAKE_BUILD_TYPE=Release"]
    subprocess.run(command, check=True)

    command = ["make"]
    subprocess.run(command, check=True)

    print(f"Local build completed successfully.")

def run_command_locally(command: list[str], stdout_file: Path = None):
    # Выполнение команды в локальном окружении
    if stdout_file is not None:
        with open(stdout_file, "w") as f:
            subprocess.run(command, check=True, stdout=f, stderr=subprocess.STDOUT)
    else:
        subprocess.run(command, check=True)

def clear_test_results_dir(directory: str):
    results_dir = os.path.join(directory, TEST_RESULT_DIRECTORY)

    if os.path.exists(results_dir) and os.path.isdir(results_dir):
        shutil.rmtree(results_dir)
        print(f"Directory {TEST_RESULT_DIRECTORY} and its contents have been removed.")
    else:
        print(f"Directory {TEST_RESULT_DIRECTORY} does not exist in {directory}.")

if __name__ == "__main__":
    files = process_d_files(BASE_RUN_DIRECTORY)
    build_local_image()  # Use local build
    clear_test_results_dir(BASE_RUN_DIRECTORY)

    try:
        for file in files:
            new_path = Path(TEST_RESULT_DIRECTORY) / Path(file).with_name(Path(file).name).with_suffix('.txt')
            new_path.parent.mkdir(parents=True, exist_ok=True)
            command = [EXECUTABLE_TARGET, Path(file).as_posix()]
            run_command_locally(command, new_path)
    finally:
        print("Test execution completed.")
