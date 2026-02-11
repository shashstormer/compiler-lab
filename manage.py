#!/usr/bin/env python3
import os
import json
import shutil
import sys
import subprocess

PROBLEMS_FILE = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'problems.json')
TEMPLATES_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'templates')

def load_problems():
    if not os.path.exists(PROBLEMS_FILE):
        print(f"Error: {PROBLEMS_FILE} not found.")
        sys.exit(1)
    with open(PROBLEMS_FILE, 'r') as f:
        return json.load(f)

def list_problems(problems):
    print("\nAvailable Problems:")
    for pid in sorted(problems.keys(), key=int):
        print(f"{pid}. {problems[pid]['title']}")

def get_user_choice(options, prompt="Select an option: "):
    while True:
        choice = input(prompt).strip()
        if choice in options:
            return choice
        print("Invalid selection. Please try again.")

def copy_template(problem_id, problem_data, subproblem_id=None):
    src_dir = os.path.join(TEMPLATES_DIR, problem_data['dir'])
    
    if subproblem_id and str(subproblem_id) != "0":
       sub_dir = os.path.join(src_dir, str(subproblem_id).replace(" ", "_").lower())
       if os.path.isdir(sub_dir):
           src_dir = sub_dir
       else:
            pass

    if not os.path.exists(src_dir):
        print(f"Error: Template directory {src_dir} does not exist yet.")
        return

    dest_name = f"ex{problem_id}"
    if subproblem_id and subproblem_id != "0":
        dest_name += f"_sub{subproblem_id}"
    
    dest_path = os.path.join(os.getcwd(), dest_name)
    
    if os.path.exists(dest_path):
        overwrite = input(f"Directory {dest_name} already exists. Overwrite? (y/n): ").lower()
        if overwrite != 'y':
            print("Aborted.")
            return
        shutil.rmtree(dest_path)

    shutil.copytree(src_dir, dest_path)
    print(f"\nSuccess! Created {dest_name} in current directory.")
    print(f"cd {dest_name} to check the files.")

def main():
    problems = load_problems()
    
    list_problems(problems)
    
    pid = get_user_choice(problems.keys(), "\nSelect problem number (1-12): ")
    problem = problems[pid]
    
    print(f"\nSelected: {problem['title']}")
    print(f"Description: {problem['description']}")
    
    sub_id = None
    if problem.get('subproblems'):
        print("\nSub-problems:")
        print("0. All")
        for sp_id, sp_desc in problem['subproblems'].items():
            print(f"{sp_id}. {sp_desc}")
        
        valid_subs = ['0'] + list(problem['subproblems'].keys())
        sub_id = get_user_choice(valid_subs, "Select sub-problem: ")
        
    action_options = ['1', '2']
    print("\nActions:")
    print("1. Generate (Copy files to CWD)")
    
    action = '1' 

    if action == '1':
        copy_template(pid, problem, sub_id)
    
if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\nExiting...")
        sys.exit(0)
