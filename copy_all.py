import sys
import os
import shutil

SCRIPT_DIR = os.path.abspath(os.path.dirname(os.path.realpath(__file__)))
TF2_ROOT_PATH = None

def copyRecursive(src, dest):
	os.makedirs(dest, exist_ok=True)

	for entry in os.listdir(src):
		if entry == ".git":
			continue

		fullSrc = os.path.join(src, entry)
		fullDest = os.path.join(dest, entry)

		if os.path.isfile(fullSrc):
			print("Copying", fullSrc, "to", fullDest)
			shutil.copy2(fullSrc, fullDest)
		elif os.path.isdir(fullSrc):
			copyRecursive(fullSrc, fullDest)

def copy(addonsFolder):
	dest = os.path.join(TF2_ROOT_PATH, "tf", "addons")

	print("Copying", addonsFolder, "to", dest)
	copyRecursive(addonsFolder, dest)

def main():
	global TF2_ROOT_PATH

	if len(sys.argv) < 2:
		print("Usage: copy_all.py <path_to_server_root>", file=sys.stderr)
		print("Server root is where the srcds executable resides.", file=sys.stderr)
		sys.exit(1)

	TF2_ROOT_PATH = sys.argv[1]

	if not os.path.isdir(TF2_ROOT_PATH):
		print("Server root", TF2_ROOT_PATH, "was not a valid directory.", file=sys.stderr)
		sys.exit(1)

	copy(os.path.join(SCRIPT_DIR, "core", "addons"))

	modesDir = os.path.join(SCRIPT_DIR, "modes")

	for entry in os.listdir(modesDir):
		if os.path.isdir(os.path.join(modesDir, entry, "addons")):
			copy(os.path.join(modesDir, entry, "addons"))

if __name__ == "__main__":
	main()
