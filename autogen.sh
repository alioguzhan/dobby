echo "Preparing build environment...";
aclocal && autoreconf -vfi && automake --add-missing
