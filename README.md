# Shell en C y extensiones de xv6 (RISC-V)

**Programación de sistemas operativos:** intérprete de comandos, sincronización y planificación de procesos.

Conjunto de trabajos prácticos realizados para **Sistemas Operativos (FAMAF – UNC, 2024)**. El repositorio conserva tres laboratorios: una shell en C, extensiones de sincronización para el sistema operativo educativo xv6 y una implementación de planificación **Multilevel Feedback Queue (MLFQ)**.

> El repositorio mantiene el nombre original de la materia. Los proyectos están organizados como `Lab 1`, `Lab 2` y `Lab 3`.

## Contenido

| Laboratorio | Proyecto | Conceptos |
| --- | --- | --- |
| [Lab 1](./Lab%201) | **Shell de comandos en C** | Procesos, ejecución de comandos, pipes y parsing |
| [Lab 2](./Lab%202) | **Semáforos en xv6-RISC-V** | Sincronización y llamadas al sistema |
| [Lab 3](./Lab%203) | **Planificador MLFQ** | Scheduling, prioridades y planificación de CPU |

**Tecnologías:** C, Linux, Bash, Make, xv6-RISC-V y herramientas de compilación/emulación.

## Organización y una aclaración importante

Este repositorio contiene tanto **material inicial (`kickstart`)** como **entregas empaquetadas (`.tar.gz`)**. Algunas rutas que parecen carpetas de proyectos son **referencias Git a otros repositorios (gitlinks)** y no contienen el código fuente directamente en este repositorio; tampoco hay un archivo `.gitmodules` en la raíz que permita reconstruirlas automáticamente con `git submodule update`.

Para recuperar las entregas archivadas, usá:

- **Shell:** `Lab 1/so24lab1g41.tar.gz`
- **Semáforos:** `Lab 2/so24lab2g41.tar.gz`
- **MLFQ:** `Lab 3/lab3.tar.gz`

La carpeta `Lab 1/kickstart/kickstart/` **no es la entrega terminada**: contiene archivos de base con partes sin implementar. No debe confundirse con el código del trabajo final.

## Cómo compilar y ejecutar

Las instrucciones siguientes explican cómo **extraer y localizar** las entregas del repositorio. Los comandos de ejecución dependen del `Makefile` contenido en cada archivo; **no se comprobó la compilación de los tres laboratorios en una instalación limpia**.

### Preparación

En Linux (Ubuntu/Debian), como punto de partida:

```bash
sudo apt update
sudo apt install git build-essential make pkg-config libglib2.0-dev check
```

Para **xv6-RISC-V** también necesitás **QEMU con soporte RISC-V** y un **compilador cruzado RISC-V** compatible con la versión de xv6 del laboratorio. Los nombres y versiones exactos dependen de tu distribución y del entorno del trabajo práctico.

Cloná el repositorio:

```bash
git clone https://github.com/SantinoBianco/SistemasOperativos-2024.git
cd SistemasOperativos-2024
```

### Lab 1 — Shell en C

Extraé el paquete de la entrega:

```bash
cd "Lab 1"
mkdir -p entrega-shell
tar -xzf so24lab1g41.tar.gz -C entrega-shell
find entrega-shell -name Makefile
```

Entrá en el directorio que contenga el **Makefile de la shell** (el que genere el ejecutable `mybash`). Desde ese directorio:

```bash
make
./mybash
```

El `Makefile` del `kickstart` existente en este repositorio define `mybash` como ejecutable y dispone de los objetivos `make test`, `make test-command` y `make test-parsing`. **Comprobá el Makefile de la entrega extraída antes de asumir que tiene exactamente los mismos objetivos.**

Para volver a la raíz del repositorio desde `Lab 1`: `cd ..`.

### Lab 2 — Semáforos sobre xv6

Desde la raíz del repositorio:

```bash
cd "Lab 2"
mkdir -p entrega-semaforos
tar -xzf so24lab2g41.tar.gz -C entrega-semaforos
find entrega-semaforos -name Makefile
```

Localizá el directorio de **xv6-RISC-V** que contenga el `Makefile`. En un árbol xv6 compatible con el laboratorio, el arranque habitual bajo emulación es:

```bash
make qemu
```

El repositorio también incluye [`xv6-riscv-semaphore.patch`](./Lab%202/xv6-riscv-semaphore.patch), un archivo de diferencias relativo a xv6. **No se debe aplicar automáticamente sobre cualquier versión de xv6**: puede requerir exactamente el código base del laboratorio.

### Lab 3 — Planificación MLFQ

Desde la raíz:

```bash
cd "Lab 3"
mkdir -p entrega-mlfq
tar -xzf lab3.tar.gz -C entrega-mlfq
find entrega-mlfq -name Makefile
```

Buscá dentro del paquete el árbol xv6 y su `Makefile`. Si dispone del objetivo `qemu` y está instalado el entorno RISC-V correspondiente, el comando habitual es:

```bash
make qemu
```

En los laboratorios basados en xv6, la interacción se realiza desde la consola del sistema dentro de QEMU. Para las pruebas o comandos específicos del planificador, consultá los archivos incluidos en la entrega extraída.

## Habilidades trabajadas

- **Programación de bajo nivel en C** y manejo de procesos.
- **Concurrencia y sincronización** mediante semáforos.
- **Modificaciones al sistema operativo xv6** y ejecución en un entorno emulado.
- **Planificación de CPU** con políticas MLFQ.

## Mejoras de presentación pendientes

Los archivos `.tar.gz` preservan las entregas, pero dificultan revisarlas directamente desde GitHub. Una mejora futura es **extraer cada entrega a una carpeta versionada con su propio README y Makefile**, sin mezclarla con el `kickstart`, para que el código sea navegable y reproducible sin pasos manuales.

---

Proyecto académico de Sistemas Operativos, FAMAF – Universidad Nacional de Córdoba. Las instrucciones se basan en la estructura y los archivos del repositorio. No implican que las compilaciones hayan sido probadas recientemente.
