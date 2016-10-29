# Change Log
All notable changes to this project will be documented in this file.

## [Unreleased]
### Added
- Added this change log CHANGELOG.md
- Added optional command line parameter to the stream test client tool in order to specify the source address

### Removed
- Removed changelog information from file README.md

### Fixed
- Changed INSTALL file reflecting use of git submodules


## [1.1] - 2016-10-06
### Added
- Works well with s-net(r) BASE release 3.6
- Added AUTHORS file

### Changed
- Makes use of git submodules for "externals"
- Renamed all main.cpp files to reflect to which tool each belongs to
- All tools were migrated to the asynchronous connect logic
- Minor changes in the structure of the project

### Fixed
- Fixed minor issues within the documentation
- Fixed invoke of shutdown() on TCP sockets to be closed


## [1.0] - 2016-08-23
### Added
- First tested version without any open issues


[Unreleased]: https://github.com/Strunzdesign/snet-tools/compare/v1.1...HEAD
[1.1]: https://github.com/Strunzdesign/snet-tools/compare/v1.0...v1.1
