import platform;
import subprocess;


LIBRARY_NAME_LINUX = "libprimary_image_color.so"
LIBRARY_NAME_DARWIN = "libprimary_image_color.a"
LIBRARY_NAME_WINDOWS = "libprimary_image_color.lib"

RED_TEXT = "\033[30m"
GREEN_TEXT = "\033[32m"

def print_colored(text, color_code):
    print(f"{color_code}{text}\033[0m")

def check_if_rust_is_installed():
    try:
        result = subprocess.run(['cargo', '--version'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        if result.returncode == 0:
            return True
    except FileNotFoundError:
        return False
    return False

def check_if_nodejs_is_installed(): 
    try:
        result = subprocess.run(['npm', '--version'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        if result.returncode == 0:
            return True
    except FileNotFoundError:
        return False
    return False

def get_os_info():
    os_system = platform.system()
    os_release = platform.release()
    os_version = platform.version()
    os_arch = platform.architecture()

    print(f"OS: {os_system}\nOS Release: {os_release}\nOS version: {os_version}\nOS Arch: {os_arch}")

    return os_system

def check_if_rust_and_node_are_installed():
    is_rust_installed = check_if_rust_is_installed()
    is_nodejs_installed = check_if_nodejs_is_installed()

    if is_rust_installed:
        print_colored("rust is installed!", GREEN_TEXT)
    else:
        print_colored("Rust is not installed", RED_TEXT)

    if is_nodejs_installed:
        print_colored("nodejs is installed", GREEN_TEXT)
    else:
        print_colored("nodejs is not installed!", RED_TEXT)

    if is_nodejs_installed & is_nodejs_installed:
        return True
    else:
        return False
    

def build_library():
    try:
        
        print('please be patient, building might take a while')

        result = subprocess.run(
            ['cargo', 'build', '--release', '--manifest-path', './primary-color/Cargo.toml'],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True  # Capture the output as a string
        )

        print("Standard Output:\n", result.stdout)
        print("Standard Error:\n", result.stderr)

        # Check if the build was successful
        if result.returncode == 0:
            print("Rust Build successful")
        else:
            print("Rust Build failed")
    except Exception as e:
        print(f"An error occurred: {e}")

def move_library(library_name):
    try: 
        subprocess.run(['mkdir', 'lib'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        subprocess.run(['mv', f'./primary-color/target/release/{library_name}', './lib'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        
    except Exception as e:
        print(f'error: {e}')

def node_gyp_configure():
    try:
        result = subprocess.run(['node-gyp', 'configure'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

        if result.returncode == 0:
            print("Configured successfully")
        else:
            print("Configuration failed")

    except Exception as e: 
        print(f"An error occurred: {e}")

def node_gyp_build():
    try:
        result = subprocess.run(['node-gyp', 'build'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

        
        if result.returncode == 0:
            print("node-gyp build successfully")
        else:
            print("node-gyp-build failed")

    except Exception as e: 
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    os_system = get_os_info()

    if os_system == "Linux":
        result = check_if_rust_and_node_are_installed()

        if result:
            build_library()
            move_library(LIBRARY_NAME_LINUX)
            node_gyp_configure()
            node_gyp_build()
            
        else:
            print("failed :(")
    else: 
        print("unsupported")


