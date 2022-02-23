import sys
import os
import subprocess

SCRIPT_DIR = os.path.abspath(os.path.dirname(os.path.realpath(__file__)))
SPCOMP_PATH = None

def compile(relPath):
	scriptPath = os.path.join(SCRIPT_DIR, relPath)
	sourcemodIncludeDir = os.path.join(os.path.dirname(SPCOMP_PATH), "include")
	fileName, fileExt = os.path.splitext(os.path.basename(scriptPath))
	scriptingDir = os.path.dirname(scriptPath)
	pluginsDir = os.path.join(os.path.dirname(scriptingDir), "plugins")
	pluginOutputFile = os.path.join(pluginsDir, f"{fileName}.smx")
	pluginIncludeDir = os.path.join(scriptingDir, "include")
	pluginLibrariesDir = os.path.join(scriptingDir, "libraries")
	arenaRouletteIncludeDir = os.path.join(SCRIPT_DIR, "core", "addons", "sourcemod", "scripting", "include")

	if not os.path.isdir(pluginsDir):
		os.makedirs(pluginsDir, exist_ok=True)

	print("Compiling:", scriptPath)

	args = \
	[
		SPCOMP_PATH,
		scriptPath,
		f"-o{pluginOutputFile}",
		f"-i{sourcemodIncludeDir}",
		f"-i{pluginIncludeDir}",
		f"-i{scriptingDir}",
		f"-i{pluginLibrariesDir}",
		f"-i{arenaRouletteIncludeDir}",
		"-O2",
		"-v2",
		"-E"
	]

	result = subprocess.run(args)

	return result.returncode == 0

def main():
	global SPCOMP_PATH

	if len(sys.argv) < 2:
		print("Usage: build_all.py <path_to_spcomp>", file=sys.stderr)
		sys.exit(1)

	SPCOMP_PATH = sys.argv[1]

	if not compile(os.path.join("core", "addons", "sourcemod", "scripting", "arena_roulette.sp")):
		print("Core plugin failed to compile", file=sys.stderr)
		sys.exit(1)

	modesDir = os.path.join(SCRIPT_DIR, "modes")

	failed = []

	for entry in os.listdir(modesDir):
		if os.path.isdir(os.path.join(modesDir, entry)):
			if not compile(os.path.join("modes", entry, "addons", "sourcemod", "scripting", f"armode_{entry}.sp")):
				failed.append(entry)

	if len(failed) > 0:
		print("The following modes failed to compile:", ", ".join(failed), file=sys.stderr)
		sys.exit(1)

if __name__ == "__main__":
	main()
