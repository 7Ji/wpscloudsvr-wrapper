# wpscloudsvr wrapper
Wraps around wpscloudsvr to provide a system tray icon to close it easily.

See also: [aur/wps-office-365](https://aur.archlinux.org/packages/wps-office-365)

## Build
Needs the following dependencies:
- libappindicator-gtk3
- gtk3 or gtk4

Run the following command to build:
```
make
```

## Install
Run the following command to install:
```
sudo make install
```
For a "fake root", e.g. in PKGBUILD:
```
make install DESTDIR="${pkgdir}"
```

## License
**wpscloudsvr-wrapper**, to wrap around wpscloudsvr to provide a system tray icon to close it easily.

Copyright (C) 2024-present Guoxin "7Ji" Pu

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.