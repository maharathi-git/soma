# soma
a calendar based on panchangam.
This application uses gtk3 and meson to build

# build essential for ubuntu
apt install build-essential libgtk-3-dev libglib2.0-dev meson ninja-build
meson setup builddir
ninja -C builddir
