# GuitarTabTransformer

## Docker

```sh
docker run --expose 8080 -p 8080:8080 --cap-add=SYS_PTRACE --security-opt seccomp=unconfined -it jbmaguire/gtt:latest bash
```