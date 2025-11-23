# Harmony

Harmony is essentially a C11 standard library extension for my personal projects

## Usage

### Harmony Library

The file harmony.h contains only types, macros, and inline functions, so can be
simply included in any C11 project. The other harmony files are independent
STB-style modules which only rely on harmony.h. They can be incorporated into
the build system by defining the macro HARMONY_IMPLEMENTATION_MODULENAME
(replacing MODULENAME with the name of the module) in exactly one file:

```c
#define HARMONY_IMPLEMENTATION_MODULENAME
#include "harmony_modulename.h"
```

Alternatively, the whole library can be built with a file that looks like this,
defining HARMONY_IMPLEMENTATION_ALL:

```c
#define HARMONY_IMPLEMENTATION_ALL
#include "harmony_audio.h"
#include "harmony_containers.h"
#include "harmony_files.h"
#include "harmony_graphics.h"
#include "harmony_math.h"
```

### Scripts and Tests

The harmony tests and utility scripts can be built using the build.sh script.
The directory build/ is created with executables for the tests and each script:

```sh
./build.sh
```

On Windows, the equivalent is build.bat:

```bat
build.bat
```

