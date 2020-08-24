# Rofi plugins 'specialchars'

Run rofi like:

```bash
    rofi -theme arthur -modi "emojis,kaomojis" -show emojis
```

## Compilation

### Dependencies

| Dependency | Version         |
|------------|-----------------|
| rofi 	     | 1.4 (or git)	   |

### Installation

**rofi-plugin-specialchars** uses autotools as build system. If installing from
git, the following steps should install it:

```bash
$ autoreconf -i
$ mkdir build
$ cd build/
# (prefix example for debian)
$ ../configure --prefix=/usr/lib/x86_64-linux-gnu
$ make
$ make install
```
