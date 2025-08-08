# Othello Game

## 프로젝트 소개

이 프로젝트는 언리얼 엔진 5.5 C++을 사용하여 개발한 오델로(리버시) 게임입니다. UMG를 활용하여 게임의 UI를 구현하였으며, 플레이어와 AI가 대결하는 턴제 방식으로 진행됩니다.

## 주요 기능

*   **게임 설정:** 게임 시작 화면에서 보드 크기와 제한 시간을 설정할 수 있습니다.
*   **게임 플레이:**
    *   플레이어(흰색 돌)와 AI(검은색 돌)가 번갈아가며 돌을 놓습니다.
    *   돌을 놓을 수 있는 위치는 반투명한 녹색 원으로 표시됩니다.
    *   제한 시간이 있으며, 시간이 초과되면 자동으로 턴이 넘어갑니다.
    *   둘 곳이 없으면 턴이 자동으로 상대방에게 넘어갑니다.
*   **AI:** Min-Max 알고리즘을 사용하여 AI의 다음 수를 계산합니다.
*   **게임 종료:**
    *   모든 칸이 채워지거나, 한쪽 색상의 돌이 모두 사라지거나, 양쪽 모두 둘 곳이 없을 때 게임이 종료됩니다.
    *   게임 종료 시 점수와 함께 승리/패배/무승부 결과가 표시됩니다.

## 클래스 상속 구조

```
- UObject
  - UGameInstance
    - UOthelloInstance
  - UUserWidget
    - UIntro
    - UPlayGame
    - UGameOver
    - UBoard
    - UBoardPart
    - UNumberWidget
- AActor
  - AGameModeBase
    - AOthelloGameModeBase
  - APlayerController
    - AMouseController
```

## 주요 클래스 기능 설명

### `UGameInstance`

*   **`UOthelloInstance`**: 게임의 전반적인 상태(보드 크기, 제한 시간, 플레이어 점수 등)를 관리하는 싱글톤 클래스입니다.

### `UUserWidget`

*   **`UIntro`**: 게임 시작 화면(인트로) UI를 관리합니다. 보드 크기와 제한 시간을 입력받아 `UOthelloInstance`에 저장하고, 게임 시작 및 종료 기능을 수행합니다.
*   **`UPlayGame`**: 실제 게임이 진행되는 화면의 UI와 로직을 관리합니다.
    *   `UBoard` 위젯을 생성하고 게임 보드를 초기화합니다.
    *   플레이어와 AI의 턴을 관리하고, 턴마다 제한 시간을 적용합니다.
    *   돌을 놓을 위치를 계산하고, 결과를 `UBoard`에 업데이트합니다.
    *   Min-Max 알고리즘을 통해 AI의 최적의 수를 계산합니다.
    *   게임 종료 조건을 확인하고, 게임 종료 화면으로 전환합니다.
*   **`UGameOver`**: 게임 종료 화면 UI를 관리합니다. 최종 점수와 승패 결과를 표시하고, 인트로 화면으로 돌아가는 기능을 제공합니다.
*   **`UBoard`**: 게임 보드 UI를 관리합니다. `UBoardPart` 위젯들을 UniformGridPanel에 동적으로 생성하여 보드를 구성하고, `UPlayGame`의 요청에 따라 보드 상태를 업데이트합니다.
*   **`UBoardPart`**: 보드의 각 칸을 나타내는 위젯입니다. 플레이어가 돌을 놓을 수 있는 버튼과 돌의 이미지를 포함하며, 돌을 뒤집는 애니메이션을 처리합니다.
*   **`UNumberWidget`**: 숫자를 표시하는 위젯입니다. 제한 시간 및 점수를 표시하는 데 사용됩니다.

### `AGameModeBase`

*   **`AOthelloGameModeBase`**: 게임의 흐름을 관리합니다. `UUserWidget`을 교체하여 인트로, 게임 플레이, 게임 종료 화면 간의 전환을 처리합니다.

### `APlayerController`

*   **`AMouseController`**: 마우스 커서를 항상 보이도록 설정합니다.

### 기타 구조체

*   **`FPlayerStatus`**: 플레이어의 점수, 돌을 놓을 수 있는지 여부, 현재 턴 정보를 저장합니다.
*   **`FNumberTexture`**: 0부터 9까지의 숫자 텍스처를 저장합니다.
*   **`FPos`**: 보드 위의 위치(x, y)를 나타냅니다.
*   **`EBoardState`**: 보드 칸의 상태(비어있음, 흰색 돌, 검은색 돌)를 나타내는 열거형입니다.
*   **`EDisplayState`**: 현재 게임 화면 상태(인트로, 플레이 중, 게임 오버)를 나타내는 열거형입니다.
*   **`EPlayState`**: 게임 플레이의 세부 상태를 나타내는 열거형입니다.
