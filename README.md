# Description

A fast CLI dice roller and random sampler utility.

# Installation

`git clone https://github.com/quxio/Roll.git && cd Roll && make && sudo make install`

**requirements**: \[ C++17, nil ]

# Usage

```bash
$ roll alice bob charlotte duncan -n2  # select two random items
charlotte
alice

$ roll alice bob charlotte duncan -a  # only shuffle
bob
alice
charlotte
duncan

$ echo $PATH | roll -n3 -d:  # set a custom delimiter
/usr/local/bin
/nix/var/nix/profiles/default/bin
/sbin

$ man bash | roll -n5  # 4908 lines in 110ms
any
this
the
An
one

$ cointoss  # convenience alias for `roll -c`
HEADS

$ roll 20   # when argv is 2, roll an <arg[1]>-sided die
4
```
