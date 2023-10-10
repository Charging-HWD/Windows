/*
1.HWND一般是用来定义窗口句柄的
HANDLE 是用来定义任何类型的句柄，（Handle to an object）.包括HWND(可以理解为HWND是HANDLE的一个特例，用来表示一个窗口)

2.HANDLE（句柄）是Windows操作系统中的一个概念,代表系统的内核对象，如文件句柄，线程句柄，进程句柄。
在Windows程序中，有各种各样的资源（窗口、图标、光标等），系统在创建这些资源时会为它们分配内存，并返回标示这些资源的标示号，即句柄。
系统对内核对象以链表的形式进行管理，载入到内存中的每一个内核对象都有一个线性地址，同时相对系统来说，在串列中有一个索引位置，这个索引位置就是内核对象的handle。
由于地址空间的限制，句柄所标识的内容对进程是不可见的，只能由操作系统通过进程句柄列表来进行维护。
句柄列表：每个进程都要创建一个句柄列表，这些句柄指向各种系统资源，比如信号量，线程，和文件等，进程中的所有线程都可以访问这些资源。

3.HINSTANCE的本质是模块基地址，他仅仅在同一进程中才有意义，跨进程的HINSTANCE是没有意义

4.HMODULE 是代表应用程序载入的模块，win32系统下通常是被载入模块的线性地址。

5.HINSTANCE 在win32下与HMODULE是相同的东西(只有在16位windows上，二者有所不同).
*/


//查找窗口：
HWND h_wnd = ::FindWindow(_T("MainForm"), NULL);
//创建窗口：
HWND hwnd = WindowEx::Create(NULL, L"", WS_POPUP, WS_EX_TOOLWINDOW);

//单例运行：
bool SingletonRun(const wchar_t *application)
{
    assert(application);
    if (application == NULL)
        return false;

    HANDLE hMutex = ::CreateMutexW(NULL, TRUE, application);

    if (hMutex == NULL)
        return false;

    if (::GetLastError() == ERROR_ALREADY_EXISTS)
    {
        ::CloseHandle(hMutex);
        return false;
    }
    /* 从此这个互斥信号量就被不受控地打开了，直到进程退出 */
    return true;
}

//获取文件大小
int64_t GetFileSize(const PathString &filepath)
{
    WIN32_FIND_DATAW file_data;
    HANDLE file = FindFirstFileW(filepath.c_str(), &file_data);

    if (file == INVALID_HANDLE_VALUE)
        return -1;
    LARGE_INTEGER li = { file_data.nFileSizeLow, file_data.nFileSizeHigh };
    FindClose(file);
    return li.QuadPart;
}

//动态加载dll:
HMODULE moudle_handle = ::LoadLibraryW(L"shell32.dll");

//获取模块directory：
std::wstring GetModuleDirectory(HMODULE module_handle)
{
    //DCHECK(IsModuleHandleValid(module_handle));
    std::wstring module_directory;
    if (FilePathApartDirectory(GetModulePathName(module_handle), module_directory))
        return module_directory;
    return L"";
}

//打开目录：
HINSTANCE inst = ::ShellExecute(NULL, L"open", dir.c_str(), NULL, NULL, SW_SHOW);