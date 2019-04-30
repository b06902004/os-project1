# 2019 OS Project 1 Report

## 設計
### 關於排程器（父行程）
負責排程的函式是`schedule`，它的演算法如下
1. 指定單獨的一顆處理器核心給此行程，使排程器的計算不會被子行程的執行干擾。
2. 以 ready time 的先後排序所有的子行程。
3. 跑一個 while-loop，每一個 iteration 代表一個時間點。對於每個 iteration，其演算法如下：
    1. 若現在執行中的子行程完成了，檢查是否所有的子行程都完成了；若是，則結束排程器（跳出 while-loop）。
    2. 檢查這個時間點下，是否有子行程是 ready 的；若是，create 這個子行程，並將它放入 sleeping 狀態。
    3. 考慮給定的 policy，找出接下來的一個單位時間要執行哪個子行程，稱它為`next`。
    4. 若 `next` 不是前一個單位時間中執行的子行程，則將後者放入 sleeping 狀態，並將 `next` 變成 running 狀態。
    5. 度過一單位的時間。


### 關於子行程
子行程的程式定義在`process.c`，演算法如下：
1. 指定一顆處理器核心給此行程（使得所有的子行程都在同一個核心上）。
2. 記錄開始時間。
3. 透過一個 for-loop，執行 input 要求需要執行的單位時間量。
4. 記錄結束時間。
5. 使用自己設計的系統呼叫將記錄的資料寫入 kernel ring buffer。

### 關於排程器如何達成 policy 的要求
`schedule.c` 裡的函式 `get_highest_priority_process` 會根據 input 指定的 policy，在每個時間點給出當下應該要執行的子程序 ID。以下個別說明每種 policy 的實作方式：
#### FIFO
在還未完成且已經 ready 的子行程中，選擇 ready time 最小的子行程。
#### PSJF
在還未完成且已經 ready 的子行程中，選擇 execution time 最小的子行程。
#### SJF
若執行中的子行程還未完成，就繼續執行它。否則，就如`PSJF`一樣。
#### RR
以 ready time 為順序依序各自執行 500 單位時間，若執行中的子行程還沒用完它的 quantum 就完成了，則直接換給下一個子行程。

### 關於如何 sleep / wake up 子行程
#### sleep
利用`sched_setscheduler`將目標行程的 priority 設為 `SCHED_IDLE`，此行程就會很難拿到 CPU。
#### wake up
利用`sched_setscheduler`將目標行程的 priority 設為 `SCHED_OTHER`。

### 關於自己設計的系統呼叫
1. 首先，在檔案`/arch/x86/entry/syscalls/syscall_64.tbl`中修改 system call table。在我的系統（x86）中，使用中的 system call number 最大為 547，因此我們挑了 548, 549 為自己設計的 system call 的號碼
2. 再來，在`kernel/sys.c`中，定義我們設計的 system call:
```c=
SYSCALL_DEFINE1(printk_for_user, char *, msg)
{
    char buf[256];
    long copied = strncpy_from_user(buf, msg, sizeof(buf));

    if (copied < 0 || copied == sizeof(buf))
        return -EFAULT;
    
    printk("%s", buf);
    return 0;
}

SYSCALL_DEFINE1(getnstimeofday_for_user, struct timespec *, ts)
{
    struct timespec tmp;
    getnstimeofday(&tmp);
    if (copy_to_user(ts, &tmp, sizeof(struct timespec)) != 0)
        return -EFAULT;
    return 0;
}
```
3. 重新編譯 kernel，並安裝新的 kernel。

## 執行範例測資的結果
範例測資的結果在`OS_PJ1_Test/results`。
## 比較實際結果與理論結果，並解釋造成差異的原因
依照 Time Measurement 中描述的如何測量單位時間，我們的排程器得到的數據是，一個單位時間等於 0.002093 秒。以測資 `FIFO_1.txt` 為例，令 P1 的 start time 為原點，藉由計算每個子行程的 finish time 與原點的差距，我們得到 P1 到 P5 的結束時間分別在第 529.289587343, 1054.84319086, 1575.6081464, 2103.35911944, 2609.81998544 個單位時間。觀察到當實際時間過得越久，其與理論結果的偏差即越來越大。其中一個顯著的原因是因為每兩個單位時間之間就得讓 scheduler 花上一些時間排程，這些不算在理論模擬裡的空隙時間累積下來便會使得實際結果越來越脫離理論結果。更詳細地說，scheduler 的一單位時間比實際的單位時間還久，故子行程得花上比理論上更久的時間才能換到它。

## 各組員的貢獻
我們小組開了兩次聚會來完成這次 project。
#### 李辰暘 b06902004
參與聚會兩次and寫code
#### 高為勳 b06902116
參與聚會兩次and查部分資料（全勤）
#### 張凱華 b06902050
參與聚會兩次 還有提供意見 （全勤）
#### 劉紹凱 b06902134
參與聚會一次
#### 李學翰 b06902108
參與聚會一次
#### 謝昊儒 b06902018
參與聚會零次
