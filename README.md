starcraft.asga
========

Source code and obtained results for "Learning to Select Actions in StarCraft with Genetic Algorithms"

----------

#### --English version--

Usage
-----

### Environment
1. Download **starcraft.zip** from <http://iccup.com/en/starcraft/sc_start.html>
2. Uncompress to folder **StarCraft\\**
3. Download **BWAPI.3.7.4.7z** from <https://github.com/bwapi/bwapi/releases>
4. Uncompress to folder **BWAPI.3.7.4\\**
5. Copy all items in **BWAPI.3.7.4\\Starcraft\\** to folder **StarCraft\\**
6. Copy all items in **BWAPI.3.7.4\\WINDOWS\\** to folder **%SystemRoot%\\** (usually **C:\\WINDOWS\\**)
7. Open folder **BWAPI 3.7.4\\Chaoslauncher\\**
8. Right-click **Chaoslauncher.exe** and choose "Run as administrator"
9. Before installing Visual Studio 2008, the following message pops up :
   ```
   Could not load Plugin "...\BWAPI.3.7.4\Chaoslauncher\Plugins\BWAPI_ChaosLauncherInjectord.bwl"
   Error. 因為應用程式的並列設定不正確，所以無法啟動。如需詳細資訊，請參閱應用程式事件記錄檔，或使用命令列工具 sxstrace.exe。(14001)
   ```
   Click "OK" (We do not have the pure English version of this message.)
10. For the first time, the following message pops up:
    ```
    Incorrect path to starcraft
    Change it in the right column of the settings-tab
    ```
    Click "OK"
11. In Chaoslauncher window, choose "Settings," on the right column, select **StarCraft.exe** in folder **StarCraft\\** created in Step 2 in "Starcraft - Installpath", and check "Automatically update on Startup"
12. Click "OK" on the bottom left corner, window Bestatigen pops up:
    ```
    SC-Path changed. You should restart the launcher now.
    Restart now?
    ```
    Click "Ja"
13. Chaoslaucher restarts. Check "BWAPI Injector" on the left to enable bwapi. Check "W-MODE" if running StarCraft in a window is desired.
14. Now, using bwapi to interface with a user-supplied dll is ready. The default setting is that Starcraft calls **StarCraft\\bwapi-data\\AI\\ExampleAIModule.dll**. It can be changed by clicking "config" after running Chaoslauncher as administrator.
15. Click "start" on the bottom left corner to start Starcraft.

### Experiments

##### Setting up
1. Copy folder **GA\\** to folder **StarCraft\\**
2. Copy folder **scenes\\** to folder **StarCraft\\maps\\**
3. Run Chaoslauncher as administrator, click "config" to change game_type as
   ```
   game_type = USE_MAP_SETTINGS
   ```
   and save.

##### Test 1
1. Run Chaoslauncher as administrator, click "config," change the following three settings as
   ```
   auto_menu = SINGLE_PLAYER
   auto_restart = ON
   map = maps/scenes/test1/test1map1.scm
   ```
   and save.
2. Copy **code\\test1\\ExampleAIModule\\Release\\ExampleAIModule.dll** to folder **StarCraft\\bwapi-data\\AI\\**
3. Edit **StarCraft\\GA\\Progress.txt** to contain **1 1 1**
4. Click "start" on the bottom left in Chaoslauncher, 3 actions will be adopted in Scene A for 10 battles and the results will be saved in **StarCraft\\GA\\log.txt**.
5. Back to Step 1, change **test1map1.scm** to **test1map2.scm** and repeat Steps 3 & 4 to do battles in Scene B.
6. Back to Step 1, change to **test1map3.scm** to do battles in Scene C.

##### Test 2
1. Run Chaoslauncher as administrator, click "config," change the following three settings as
   ```
   auto_menu = SINGLE_PLAYER
   auto_restart = ON
   map = maps/scenes/test2/test2map*.scm
   ```
   and save.
2. Copy **code\\test2\\ExampleAIModule\\Release\\ExampleAIModule.dll** to folder **StarCraft\\bwapi-data\\AI\\**
3. Edit **StarCraft\\GA\\Progress.txt** to contain **0 0 1**
4. Click "start" on the bottom left in Chaoslauncher to start StarCraft as well the main Genetic Algorithm. The results will be saved in **StarCraft\\GA\\log.txt**.

##### Test 3
1. Run Chaoslauncher as administrator, click "config," change the following three settings as
   ```
   auto_menu = SINGLE_PLAYER
   auto_restart = ON
   map = maps/scenes/test3/test3map*.scm
   ```
   and save.
2. Copy **code\\test3\\ExampleAIModule\\Release\\ExampleAIModule.dll** to folder **StarCraft\\bwapi-data\\AI\\**
3. Edit **StarCraft\\GA\\Progress.txt** to contain **0 0 1**
4. Click "start" on the bottom left in Chaoslauncher to start StarCraft as well the main Genetic Algorithm. The results will be saved in **StarCraft\\GA\\log.txt**.

##### Test 4
1. Run Chaoslauncher as administrator, click "config," change the following three settings as
   ```
   auto_menu = SINGLE_PLAYER
   auto_restart = ON
   map = maps/scenes/test4/test4map*.scm
   ```
   and save.
2. Copy **code\\test4\\ExampleAIModule\\Release\\ExampleAIModule.dll** to folder **StarCraft\\bwapi-data\\AI\\**
3. Edit **StarCraft\\GA\\Progress.txt** to contain **0 0 1**
4. Click "start" on the bottom left in Chaoslauncher to start StarCraft as well the main Genetic Algorithm. The results will be saved in **StarCraft\\GA\\log.txt**.

### Building the DLL

1. This project is developed in Visual Studio 2008。
2. The items in folders **BWAPI 3.7.4\\include\\** and **BWAPI 3.7.4\\lib\\** are requied.
3. Build the project as Release version.
4. The following two lines in **ExampleAIModule.cpp** controlls the speed of games and whether to display the progress. The DLLs in the repository contain these two lines. Hence, the game is runnuing faster than normal, and there is no progress display. Commenting out the two lines will render normal games.
   ```
   Broodwar->setGUI(false);
   Broodwar->setLocalSpeed(0);
   ```

### Miscellaneous
1. The three numbers in **Progress.txt** represents at generation i, individual j, battle k. For example, 1 2 3 means at generation 1, individual 2, battle 3. Specifying the three numbers can do the specific battle. Using "config" can fix the battle map, such as
   ```
   map = maps/scenes/test3/test3map1a.scm
   ```
   will battle in Scene A and
   ```
   map = maps/scenes/test3/test3map*.scm
   ```
   will battle in Scenes A, B, and C.
2. There are currently some problems with the map numbering. For example, for five maps 1 2 3 4 5, the battle will be on 1->3->5->2->4. Hence, to avoid this issue in this project, maps are duplicated as 1 1 2 2 3 3 4 4 5 5. The order is then 1->2->3->4->5.

----------

Videos mentioned in paper
---------------

> https://nclab.github.io/starcraft.asga

----------

Use
---

Please cite

> Hsu, W.-L., & Chen, Y.-p. (2016). Learning to Select Actions in StarCraft with Genetic Algorithms. <i>2016 Conference on Technologies and Applications of Artificial Intelligence (TAAI 2016)</i>.

----------

#### --中文版本--

使用說明
-----

### 環境設定
1. 從 <http://iccup.com/en/starcraft/sc_start.html> 下載 **starcraft.zip**
2. 解壓縮至 **StarCraft\\** 資料夾
3. 從 <https://github.com/bwapi/bwapi/releases> 下載 **BWAPI.3.7.4.7z**
4. 解壓縮至 **BWAPI.3.7.4\\** 資料夾
5. 將 **BWAPI.3.7.4\\Starcraft\\** 內的所有內容複製到 **StarCraft\\** 資料夾內
6. 將 **BWAPI.3.7.4\\WINDOWS\\** 內的所有內容複製到 **%SystemRoot%\\** 資料夾 (通常是 **C:\\WINDOWS\\**) 內
7. 打開 **BWAPI 3.7.4\\Chaoslauncher\\** 資料夾
8. 以右鍵按 **Chaoslauncher.exe** 選擇「以系統管理員身份執行」
9. 在安裝 Visual Studio 2008 之前，會出現例外訊息視窗，訊息內容為
   ```
   Could not load Plugin "...\BWAPI.3.7.4\Chaoslauncher\Plugins\BWAPI_ChaosLauncherInjectord.bwl"
   Error. 因為應用程式的並列設定不正確，所以無法啟動。如需詳細資訊，請參閱應用程式事件記錄檔，或使用命令列工具 sxstrace.exe。(14001)
   ```
   按下「確定」
10. 第一次執行時，會出現訊息視窗，訊息內容為
    ```
    Incorrect path to starcraft
    Change it in the right column of the settings-tab
    ```
    按下「OK」
11. 在「Chaoslauncher」組態視窗中選擇「Settings」中間右邊 Starcraft - Installpath 選取在第 2 步驟產生的 **StarCraft\\** 資料夾內的 **StarCraft.exe** 並將 "Automatically update on Startup" 打勾
12. 點選左邊偏下方的「OK」出現「Bestatigen」視窗，訊息內容為
    ```
    SC-Path changed. You should restart the launcher now.
    Restart now?
    ```
    按下「Ja」
13. Chaoslaucher 會重新執行，將左欄「BWAPI Injector」打勾便可啟用 bwapi。如果希望以視窗方式執行，可將「W-MODE」打勾。
14. 到此為止已經可以使用 bwapi 介接提供 AI 的 dll 了，預設狀況是 StarCraft 會叫用 **StarCraft\\bwapi-data\\AI\\ExampleAIModule.dll** 這個 dll 檔。若需更改，在以系統管理員身份執行 Chaoslauncher 後，點選 config 即可進行更改。
15. 點選視窗內下方左邊的「Start」即可開始執行 Starcraft。

### 進行實驗

##### 前置作業
1. 將 **GA\\** 資料夾複製到 **StarCraft\\**
2. 將 **scenes\\** 資料夾複製到 **StarCraft\\maps\\**
3. 以系統管理員身份執行 Chaoslauncher 並點選 config 將 game_type 改為
   ```
   game_type = USE_MAP_SETTINGS
   ```
   並且儲存。

##### 實驗一
1. 以系統管理員身份執行 Chaoslauncher 點選 config 並找到下面三行，分別設定為
   ```
   auto_menu = SINGLE_PLAYER
   auto_restart = ON
   map = maps/scenes/test1/test1map1.scm
   ```
   並且儲存。
2. 將 **code\\test1\\ExampleAIModule\\Release\\ExampleAIModule.dll** 複製至 **StarCraft\\bwapi-data\\AI\\**
3. 開啟 **StarCraft\\GA\\Progress.txt** 設定為 **1 1 1**
4. 點選 Chaoslauncher 左下 start 即可進入遊戲主畫面，便會自動使用三個行動在 A 場景進行 10 場戰鬥，結果會儲存在 **StarCraft\\GA\\log.txt** 中。
5. 回到步驟 1，將 **test1map1.scm** 改成 **test1map2.scm**，重複步驟 3 和 4，進行 B 場景的戰鬥。
6. 回到步驟 1，改成 **test1map3.scm**，進行 C 場景的戰鬥。

##### 實驗二
1. 以系統管理員身份執行 Chaoslauncher 點選 config 並找到下面三行，分別設定為
   ```
   auto_menu = SINGLE_PLAYER
   auto_restart = ON
   map = maps/scenes/test2/test2map*.scm
   ```
   並且儲存。
2. 將 **code\\test2\\ExampleAIModule\\Release\\ExampleAIModule.dll** 複製至 **StarCraft\\bwapi-data\\AI\\**
3. 開啟 **StarCraft\\GA\\Progress.txt** 設定為 **0 0 1**
4. 點選 Chaoslauncher 左下 start 即可進入遊戲主畫面，並開始執行 GA，結果會儲存在 **StarCraft\\GA\\log.txt** 中。

##### 實驗三
1. 以系統管理員身份執行 Chaoslauncher 點選 config 並找到下面三行，分別設定為
   ```
   auto_menu = SINGLE_PLAYER
   auto_restart = ON
   map = maps/scenes/test3/test3map*.scm
   ```
   並且儲存。
2. 將 **code\\test3\\ExampleAIModule\\Release\\ExampleAIModule.dll** 複製至 **StarCraft\\bwapi-data\\AI\\**
3. 開啟 **StarCraft\\GA\\Progress.txt** 設定為 **0 0 1**
4. 點選 Chaoslauncher 左下 start 即可進入遊戲主畫面，並開始執行 GA，結果會儲存在 **StarCraft\\GA\\log.txt** 中。

##### 實驗四
1. 以系統管理員身份執行 Chaoslauncher 點選 config 並找到下面三行，分別設定為
   ```
   auto_menu = SINGLE_PLAYER
   auto_restart = ON
   map = maps/scenes/test4/test4map*.scm
   ```
   並且儲存。
2. 將 **code\\test4\\ExampleAIModule\\Release\\ExampleAIModule.dll** 複製至 **StarCraft\\bwapi-data\\AI\\**
3. 開啟 **StarCraft\\GA\\Progress.txt** 設定為 **0 0 1**
4. 點選 Chaoslauncher 左下 start 即可進入遊戲主畫面，並開始執行 GA，結果會儲存在 **StarCraft\\GA\\log.txt** 中。

### 編譯部份
1. 本專案使用的開發環境為 Visual Studio 2008。
2. 編譯需要 **BWAPI 3.7.4\\include\\** 和 **BWAPI 3.7.4\\lib\\** 這兩個資料夾內的檔案。
3. 使用 Release 模式編譯。
4. 在 **ExampleAIModule.cpp** 中的以下兩行，控制了「遊戲速度」與「是否顯示畫面」。隨專案附上的的 dll 都包含有這兩行，也就是會加速且無畫面。如果需要正常執行過程與狀況顯示，可將以下兩行加以註解。
   ```
   Broodwar->setGUI(false);
   Broodwar->setLocalSpeed(0);
   ```

### 其他
1. **Progress.txt** 中的三個數字代表第 i 代的第 j 個體的第 k 場，例如 1 2 3 就是第 1 代的第 2 個體的第 3 場。實驗做完後，可以修改這三個數字來選擇個體，想固定地圖就在 config 更改地圖位置，例如
   ```
   map = maps/scenes/test3/test3map1a.scm
   ```
   就會只在 A 場景對戰，
   ```
   map = maps/scenes/test3/test3map*.scm
   ```
   就會依序在 A, B, C 場景對戰。
2. 目前發現地圖依序讀取時會發生跳號，例如，五張地圖 1 2 3 4 5，對戰的順序會變成 1->3->5->2->4，因此，暫時的處理方式是把地圖多複製一份變成 1 1 2 2 3 3 4 4 5 5，讀取的順序就會是 1->2->3->4->5。

----------

論文中提及之影片
---------------

> https://nclab.github.io/starcraft.asga

----------

使用本專案
---

請引用

> Hsu, W.-L., & Chen, Y.-p. (2016). Learning to Select Actions in StarCraft with Genetic Algorithms. <i>2016 Conference on Technologies and Applications of Artificial Intelligence (TAAI 2016)</i>.
