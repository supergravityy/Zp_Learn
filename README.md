# Zp_learn

WSL Ubuntu에서 zenoh-pico 예제를 빌드하는 프로젝트입니다.
CMake 3.20 이상, Ninja, C 컴파일러가 필요합니다.

## 전체 clean 빌드

최초 체크아웃 후 서브모듈을 준비합니다.

```bash
git submodule update --init --recursive
```

프로젝트 최상위 폴더에서 실행합니다.

```bash
./build.sh
```

실행 권한이 없는 체크아웃에서는 `bash build.sh`로 실행할 수 있습니다.
스크립트는 자신의 위치를 기준으로 동작하므로 다른 작업 디렉터리에서도 호출할 수 있습니다.

`App`의 `dev` 프리셋으로 CMake를 구성한 뒤, `--clean-first`로 기존 빌드 산출물을 정리하고 전체 기본 타깃을 다시 빌드합니다.
CMake 캐시는 유지하며 소스는 삭제하지 않습니다. 오류가 발생하면 즉시 중단합니다.
결과는 `App/build/dev/<예제 폴더>/<실행 파일 이름>`에 생성됩니다.

현재 예제: `00_build_check`, `01_session_peer`.
새 예제는 해당 폴더에 `CMakeLists.txt`를 만들고 `App/CMakeLists.txt`에 `add_subdirectory(...)`로 등록하면 전체 빌드에 포함됩니다.
`01_session_peer`는 세션 열기 → 상대 peer ID 조회 → Ctrl+C 종료까지의 스켈레톤입니다. 데이터 송수신은 아직 구현하지 않았습니다.

## Session 예제만 빌드·실행

```bash
cd App
cmake --preset dev
cmake --build --preset dev --target session_examples
```

두 WSL 터미널에서 각각 `App/build/dev/01_session_peer`로 이동합니다.
TCP는 첫 터미널에서 `./tcp_listener`, 다음 터미널에서 `./tcp_connector`를 실행합니다.
UDP는 각각 `./udp_peer0`, `./udp_peer1`을 실행합니다.
`[PEER]`에 상대 ID가 출력되는지 확인하고 Ctrl+C로 종료합니다.
UDP는 같은 WSL 안에서 실습하도록 `#iface=lo`를 지정했습니다.

회사와 집에서는 빌드 폴더를 복사하지 말고 각 WSL 환경에서 이 스크립트로 빌드하세요.
