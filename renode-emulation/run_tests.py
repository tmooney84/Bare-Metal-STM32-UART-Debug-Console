import os
import subprocess
import sys
import shutil
import glob

# --- Configuration ---
VENV_DIR = ".venv"
REQUIREMENTS = "requirements.txt"
TEST_FILE = "UART_debug_console.robot"

def check_prerequisites():
    print("--- Checking Prerequisites ---")
    
    # 1. Check Python Version (Requires 3.6+)
    if sys.version_info < (3, 6):
        sys.exit("Error: Python 3.6 or higher is required.")
    print(f"Python version: {sys.version.split()[0]} - OK")

    # 2. Check for Renode
    renode_cmd = shutil.which("renode-test")
    if not renode_cmd:
        sys.exit("Error: 'renode-test' not found in PATH. Please install Renode from renode.io.")
    
    # Get Renode version
    try:
        version = subprocess.check_output(["renode", "--version"], stderr=subprocess.STDOUT).decode()
        print(f"Renode {version.strip()} - OK")
    except Exception:
        print("Renode detected, but version check failed. Proceeding anyway...")

def setup_venv():
    print("\n--- Setting up Virtual Environment ---")
    if not os.path.exists(VENV_DIR):
        subprocess.run([sys.executable, "-m", "venv", VENV_DIR], check=True)
        print("Created new venv.")
    
    # Determine the pip path based on OS
    if os.name == "nt": # Windows
        pip_exe = os.path.join(VENV_DIR, "Scripts", "pip.exe")
    else: # Linux/macOS
        pip_exe = os.path.join(VENV_DIR, "bin", "pip")

    print("Installing/Updating dependencies...")
    subprocess.run([pip_exe, "install", "--upgrade", "pip"], check=True)
    subprocess.run([pip_exe, "install", "-r", REQUIREMENTS], check=True)

def run_test():
    print(f"\n--- Running Test: {TEST_FILE} ---")
    
    # 1. Locate the venv's bin/Scripts folder
    if os.name == "nt": # Windows
        venv_bin = os.path.join(os.getcwd(), VENV_DIR, "Scripts")
    else: # Linux/macOS
        venv_bin = os.path.join(os.getcwd(), VENV_DIR, "bin")

    # 2. Add the venv bin folder to the START of the PATH
    # This forces renode-test to find the 'python' and 'robot' inside the venv
    env = os.environ.copy()
    env["PATH"] = venv_bin + os.pathsep + env["PATH"]
    
    # 3. Explicitly tell Renode where the venv's site-packages are
    # (Optional but very helpful for portable Renode versions)
    env["PYTHONPATH"] = venv_bin

    try:
        # Pass our modified environment to the subprocess
        subprocess.run(["renode-test", TEST_FILE], check=True, env=env)
    except subprocess.CalledProcessError:
        print("\nTest Failed!")
        sys.exit(1)

def cleanup_logs():
    answer = input("Do you want to clean up log files? (y/N)").strip().lower()

    if answer == 'y':
        print("Cleaning up logs and reports...")

        patterns = ["*.log*", "*.html", "*.xml", "*.save"]

        files_deleted = 0
        for pattern in patterns:
            for f in glob.glob(pattern):
                try:
                    os.remove(f)
                    files_deleted += 1
                except OSError as e:
                    print(f"Error deleting {f}: {e}")
        print(f"Cleanup complete. Deleted {files_deleted} files.")
    else:
        print("Cleanup skipped.")

if __name__ == "__main__":
    check_prerequisites()
    setup_venv()
    run_test()
    cleanup_logs()