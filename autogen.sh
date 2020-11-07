echo "Preparing build environment..."
aclocal && autoheader && autoreconf -vfi && ./configure && automake --add-missing
