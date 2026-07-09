# weServer

Servidor HTTPS CRUD basado en Boost.Beast y corrutinas C++20.

## Requisitos

- CMake 3.23+
- Conan 2
- Compilador con soporte C++20 (g++-15/16 recomendado)
- OpenSSL (para certificados y dependencia de Conan)

## Build

```bash
chmod +x scripts/build.sh
./scripts/build.sh
```

O manualmente:

```bash
conan install . --build=missing -s compiler.cppstd=20 -s build_type=Debug
cmake --preset conan-debug
cmake --build --preset conan-debug
```

El script genera certificados autofirmados (`server.cert.pem`, `server.key.pem`) si no existen.

## Estructura CMake

```
CMakeLists.txt              # proyecto, opciones, instalación
cmake/
  WeServerOptions.cmake     # C++20, compile_commands, opciones de build
  WeServerDependencies.cmake
  WeServerTargets.cmake     # weServer::include, weServer::options, helpers
  WeServerInstall.cmake     # install() + export weServer::*
  WeServerConfig.cmake.in
src/
  CMakeLists.txt            # ejecutable weserver
  store/                    # weServer::store   — datos en memoria
  protocol/                 # weServer::protocol — HTTP helpers, SSL, errores
  crud/                     # weServer::crud     — handlers REST
  net/                      # weServer::net      — servidor y sesiones
test/
  CMakeLists.txt
```

Grafo de dependencias: `weserver` → `net` → `crud` → `protocol` + `store`

Opciones CMake:

| Opción | Default | Descripción |
|--------|---------|-------------|
| `WEB_SERVER_BUILD_TESTS` | ON | Compilar tests GTest |
| `WESERVER_DEVELOPER_MODE` | ON | Generar `compile_commands.json` |
| `WESERVER_WARNINGS_AS_ERRORS` | OFF | `-Werror` / `/WX` |

Instalación (opcional):

```bash
cmake --install build/Debug --prefix /opt/weserver
```

## Ejecutar

```bash
./build/Debug/weserver
```

Escucha en `https://127.0.0.1:8080`.

## Endpoints CRUD

| Método | Ruta | Descripción |
|--------|------|-------------|
| GET | `/crud/items` | Listar items |
| POST | `/crud/items` | Crear item (`{"name":"...", "description":"..."}`) |
| GET | `/crud/items/{id}` | Obtener item |
| PUT | `/crud/items/{id}` | Actualizar item |
| DELETE | `/crud/items/{id}` | Eliminar item |

Los datos viven en memoria (sin BBDD por ahora).

## Probar

Con el servidor en marcha:

```bash
python3 scripts/test_crud.py
```

## Tests unitarios

```bash
ctest --test-dir build/Debug
```

## Estilo de código

El código nuevo sigue las [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
donde aplica de forma estricta:

- **SF.1 / NL.27**: cabeceras `.h`, implementación `.cpp`
- **`#pragma once`** en cabeceras (excepción deliberada a SF.8)
- **NL.8 / NL.10**: tipos en `PascalCase`, funciones y variables en `snake_case`
- **EH.1**: llaves en todos los `if`
- **ES.45**: constantes con nombre en `constants.h`

`clang-format` (`.clang-format`) para formato mecánico.
