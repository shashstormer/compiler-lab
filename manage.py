import os
import json
import shutil
import sys
import argparse
import glob

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

def clean_generated():
    """Removes created ex* directories."""
    patterns = ["ex[0-9]*", "ex[0-9]*_sub[0-9]*"]
    found = []
    for p in patterns:
        found.extend(glob.glob(p))
    
    found = [d for d in found if os.path.exists(d)]
    
    if not found:
        print("No generated lab directories found to clean.")
        return

    print(f"Found {len(found)} directories: {', '.join(found)}")
    confirm = input("Are you sure you want to delete them? (y/n): ").lower()
    
    if confirm == 'y':
        removed_count = 0
        for d in found:
            try:
                if os.path.exists(d):
                    if os.path.isdir(d):
                        shutil.rmtree(d)
                    else:
                        os.remove(d)
                    print(f"Removed {d}")
                    removed_count += 1
                else:
                    print(f"Skipped {d} (already gone)")
            except OSError as e:
                print(f"Error removing {d}: {e}")
                
        print(f"Cleanup complete. Removed {removed_count} items.")
    else:
        print("Aborted.")

def show_help(problems, pid, sub_id=None):
    if pid not in problems:
        print(f"Error: Problem ID {pid} not found.")
        return

    p = problems[pid]
    print(f"\n=== {p['title']} ===")
    print(f"Description: {p['description']}")
    
    if 'usage' in p:
        u = p['usage']
        print(f"\n[Explanation]\n{u.get('explanation', 'N/A')}")
        print(f"\n[Algorithm]\n{u.get('algorithm', 'N/A')}")
        print(f"\n[How to Run]\n{u.get('run', 'N/A')}")
    
    if p.get('subproblems'):
        print("\n[Sub-problems]")
        for sid, desc in p['subproblems'].items():
            print(f"  {sid}: {desc}")
    
    if sub_id:
       if sub_id in p.get('subproblems', {}):
           print(f"\nSpecific help for subproblem {sub_id}: (See code comments for details)")
       else:
           print(f"\nWarning: Subproblem {sub_id} not found.")

def copy_template(problem_id, problem_data, subproblem_id=None):
    src_dir = os.path.join(TEMPLATES_DIR, problem_data['dir'])
    
    if subproblem_id and str(subproblem_id) != "0":
        sub_dir_path = os.path.join(src_dir, str(subproblem_id))
        if os.path.isdir(sub_dir_path):
             src_dir = sub_dir_path
        else:
             sub_dir_path_legacy = os.path.join(src_dir, str(subproblem_id).replace(" ", "_").lower())
             if os.path.isdir(sub_dir_path_legacy):
                 src_dir = sub_dir_path_legacy

    if not os.path.exists(src_dir):
        print(f"Error: Template directory {src_dir} does not exist yet.")
        return

    dest_name = f"ex{problem_id}"
    if subproblem_id and str(subproblem_id) != "0":
        dest_name += f"_sub{subproblem_id}"
    
    dest_path = os.path.join(os.getcwd(), dest_name)
    
    if os.path.exists(dest_path):
        overwrite = input(f"Directory {dest_name} already exists. Overwrite? (y/n): ").lower()
        if overwrite != 'y':
            print("Aborted.")
            return
        try:
            if os.path.isdir(dest_path):
                shutil.rmtree(dest_path)
            else:
                os.remove(dest_path)
        except OSError as e:
            print(f"Error removing existing directory {dest_path}: {e}")
            return

    try:
        shutil.copytree(src_dir, dest_path)
        print(f"\nSuccess! Created {dest_name} in current directory.")
        print(f"cd {dest_name} to check the files.")
    except OSError as e:
        print(f"Error copying template: {e}")

def interactive_mode(problems):
    list_problems(problems)
    
    while True:
        pid = input("\nSelect problem number (1-12) or 'q' to quit: ").strip()
        if pid.lower() == 'q':
            sys.exit(0)
        if pid in problems:
            break
        print("Invalid selection.")

    problem = problems[pid]
    sub_id = None
    
    if problem.get('subproblems'):
        print("\nSub-problems:")
        for sp_id, sp_desc in problem['subproblems'].items():
            print(f"{sp_id}. {sp_desc}")
        
        while True:
            sub_id = input("Select sub-problem (or 0 to skip): ").strip()
            if sub_id == '0' or sub_id in problem['subproblems']:
                if sub_id == '0': sub_id = None
                break
            print("Invalid sub-problem.")

    copy_template(pid, problem, sub_id)


def main():
    parser = argparse.ArgumentParser(description="Compiler Design Lab Manager")
    subparsers = parser.add_subparsers(dest="command", help="Command to run")

    parser_gen = subparsers.add_parser("generate", help="Generate lab templates")
    parser_gen.add_argument("problem", nargs="?", help="Problem ID (1-12)")
    parser_gen.add_argument("subproblem", nargs="?", help="Sub-problem ID")

    parser_list = subparsers.add_parser("list", help="List available problems")

    parser_help = subparsers.add_parser("help", help="Show help for a problem")
    parser_help.add_argument("problem", help="Problem ID")
    parser_help.add_argument("subproblem", nargs="?", help="Sub-problem ID")
    parser_clean = subparsers.add_parser("clean", help="Remove generated lab directories")

    args = parser.parse_args()
    problems = load_problems()

    if args.command == "list":
        list_problems(problems)
    elif args.command == "clean":
        clean_generated()
    elif args.command == "help":
        show_help(problems, args.problem, args.subproblem)
    elif args.command == "generate":
        if args.problem:
            if args.problem not in problems:
                print("Invalid problem ID.")
                sys.exit(1)
            copy_template(args.problem, problems[args.problem], args.subproblem)
        else:
            interactive_mode(problems)
    else:
        interactive_mode(problems)

if __name__ == "__main__":
    main()
