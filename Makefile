.SILENT: install
app:
	clang -Wall -Werror -Wextra -Wno-unused-parameter -o dobby src/dobby.c src/start.c src/stop.c src/list.c src/utils.c src/colorize.c 
install:
	echo "⏳ Installing Dobby to /usr/local/bin..." && cp ./dobby /usr/local/bin/dobby && echo "✅ Done."
test:
	bash ./tests/tests.sh