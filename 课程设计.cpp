// 课程设计.cpp : 定义应用程序的入口点。
//
#include "include.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
vector<Student> stuset;
//控件句柄
static HWND
hboxlist,
hsearchwd,
hsearchbtn,
hno, 
hnotext, 
hnobtn,
hname,
hnametext,
hnamebtn,
hadress,
hpopwin,
hpopwinbtn,
hadresstext,
hadressbtn,
hold,
holdtext,
holdbtn,
hmath,
hmathtext,
hmathbtn,
hcpp,
hrevisebtn,
hcpptext,
hcancelbtn,
hmanbtn,
hsexbox,
hwomanbtn,
hmantext,
hwomantext,
hcppbtn;

static char text[256];
static int stuidx = -1;

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void searchword(const char*);
void savedata();
void loaddata();
void showmsg(const int&);
bool sortmath(const Student& a, const Student& b);
bool sortcpp(const Student& a, const Student& b);
void drowgraph(HDC& hdc);
bool is_file_empty(const std::string& filename);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。
    loaddata();
    searchword("");
    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口··
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        hboxlist = CreateWindow(TEXT("LISTBOX"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY | WS_VSCROLL,
            0, 20, 250, 500, hWnd, (HMENU)ID_STUDENTLISTBOX, hInst, NULL);

        hsexbox = CreateWindow(TEXT("STATIC"), TEXT(" 性别:"), WS_CHILD | WS_VISIBLE | SS_CENTER,
            350, 135, 250, 60, hWnd, (HMENU)ID_SEX, hInst, NULL);

        hmantext = CreateWindow(TEXT("STATIC"), TEXT("男"), WS_CHILD | WS_VISIBLE,
            93, 20, 20, 20, hsexbox, (HMENU)ID_MANTEXT, hInst, NULL);
        hwomantext = CreateWindow(TEXT("STATIC"), TEXT("女"), WS_CHILD | WS_VISIBLE,
            177, 20, 20, 20, hsexbox, (HMENU)ID_WOMANTEXT, hInst, NULL);

        hsearchwd = CreateWindow(TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
            0, 0, 210, 20, hWnd, (HMENU)ID_EDITSEARCH, hInst, NULL);
        hsearchbtn = CreateWindow(TEXT("BUTTON"), L"搜索", WS_CHILD | WS_VISIBLE | WS_BORDER,
            210, 0, 40, 20, hWnd, (HMENU)ID_BUTTONSEARCH, hInst, NULL);
        //静态的窗口其实可以换成TextOut
        hnotext = CreateWindow(TEXT("STATIC"), TEXT(" 学号:"), WS_CHILD | WS_VISIBLE,
            300, 50, 42, 20, hWnd, (HMENU)ID_NOTEXT, hInst, NULL);
        hno = CreateWindow(TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
            350, 50, 250, 20, hWnd, (HMENU)ID_EDITNO, hInst, NULL);
        hnobtn = CreateWindow(TEXT("BUTTON"), L"删除", WS_CHILD | WS_VISIBLE | WS_BORDER,
            350, 390, 40, 20, hWnd, (HMENU)ID_DESTORY, hInst, NULL);

        hnametext = CreateWindow(TEXT("STATIC"), TEXT(" 姓名:"), WS_CHILD | WS_VISIBLE,
            300, 100, 42, 20, hWnd, (HMENU)ID_NAMETEXT, hInst, NULL);
        hname = CreateWindow(TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
            350, 100, 250, 20, hWnd, (HMENU)ID_EDITNAME, hInst, NULL);

        hnamebtn = CreateWindow(TEXT("BUTTON"), L"C++排序", WS_CHILD | WS_VISIBLE | WS_BORDER,
            430, 410, 80, 20, hWnd, (HMENU)ID_CPPSORT, hInst, NULL);
        
        holdtext = CreateWindow(TEXT("STATIC"), TEXT(" 年龄:"), WS_CHILD | WS_VISIBLE,
            300, 210, 42, 20, hWnd, (HMENU)ID_OLDTEXT, hInst, NULL);
        hold = CreateWindow(TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
            350, 210, 250, 20, hWnd, (HMENU)ID_EDITOLD, hInst, NULL);

        holdbtn = CreateWindow(TEXT("BUTTON"), L"随机生成", WS_CHILD | WS_VISIBLE | WS_BORDER,
            510, 390, 80, 20, hWnd, (HMENU)ID_RANDSUMMON, hInst, NULL);
        holdtext = CreateWindow(TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 
            590, 390, 20, 20, hWnd, (HMENU)ID_RANDSUMMONTEXT, hInst, NULL);

        hadresstext = CreateWindow(TEXT("STATIC"), TEXT(" 地址:"), WS_CHILD | WS_VISIBLE,
            300, 260, 42, 20, hWnd, (HMENU)ID_ADRESSTEXT, hInst, NULL);
        hadress = CreateWindow(TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
            350, 260, 250, 20, hWnd, (HMENU)ID_EDITADRESS, hInst, NULL);
        hadressbtn = CreateWindow(TEXT("BUTTON"), L"增加", WS_CHILD | WS_VISIBLE | WS_BORDER,
            390, 390, 40, 20, hWnd, (HMENU)ID_ADD, hInst, NULL);

        hmathtext = CreateWindow(TEXT("STATIC"), TEXT(" 数学:"), WS_CHILD | WS_VISIBLE,
            300, 310, 42, 20, hWnd, (HMENU)ID_MATHTEXT, hInst, NULL);
        hmath = CreateWindow(TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
            350, 310, 250, 20, hWnd, (HMENU)ID_EDITMATH, hInst, NULL);
        hmathbtn = CreateWindow(TEXT("BUTTON"), L"数学排序", WS_CHILD | WS_VISIBLE | WS_BORDER,
            350, 410, 80, 20, hWnd, (HMENU)ID_MATHSORT, hInst, NULL);

        hpopwin = CreateWindow(TEXT("POPUP"), L"添加失败", WS_CHILD | WS_VISIBLE | WS_BORDER,
            400, 450, 160, 90, hWnd, (HMENU)ID_POP1, hInst, NULL);
        hpopwinbtn = CreateWindow(TEXT("POPUP"), L"确认", WS_CHILD | WS_VISIBLE | WS_BORDER,
            120, 70, 40, 20, hpopwin, (HMENU)ID_POPBTN1, hInst, NULL);

        hcpptext = CreateWindow(TEXT("STATIC"), TEXT(" C++:"), WS_CHILD | WS_VISIBLE,
            300, 360, 42, 20, hWnd, (HMENU)ID_CPPTEXT, hInst, NULL);
        hcpp = CreateWindow(TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
            350, 360, 250, 20, hWnd, (HMENU)ID_EDITCPP, hInst, NULL);
        hcppbtn = CreateWindow(TEXT("BUTTON"), L"保存", WS_CHILD | WS_VISIBLE | WS_BORDER,
            470, 390, 40, 20, hWnd, (HMENU)ID_COMFIRM, hInst, NULL);
        hrevisebtn = CreateWindow(TEXT("BUTTON"), L"修改", WS_CHILD | WS_VISIBLE | WS_BORDER,
            430, 390, 40, 20, hWnd, (HMENU)ID_REVISE, hInst, NULL);
        hcancelbtn = CreateWindow(TEXT("BUTTON"), L"取消", WS_CHILD | WS_VISIBLE | WS_BORDER,
            510, 410, 40, 20, hWnd, (HMENU)ID_CANCEL, hInst, NULL);

        hmanbtn = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            423, 155, 20, 20, hWnd, (HMENU)ID_MANBTN, hInst, NULL);
        hwomanbtn = CreateWindow(TEXT("BUTTON"), NULL, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
            507, 155, 20, 20, hWnd, (HMENU)ID_WOMANBTN, hInst, NULL);

        break;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            int event = HIWORD(wParam);
            // 分析菜单选择:
            
            switch (wmId)
            {
                //随机生成n个学生
            case ID_RANDSUMMON:
            {
                static vector<string> firstName = {"王", "李", "赵", "何", "魏", "谢", "刘", "郭", "严", "陈", "刁", "饶", "朱", "黄"};
                static vector<string> lastName = {"三", "晶晶", "静", "大壮", "萍", "丹丹", "桐桐", "珊", "海", "焕焕", "朗朗", "芊芊", "聪", "芳芳"};
                static vector<string> firstadress = {"盖利德大学，", "火星，", "冥王星，", "竹苑5，", "菊苑2，", "兰苑4，", "梅苑3，"};
                static vector<string> midadress = {"七度空间，", "王城街道，", "图书馆，", "丽湖旁，", "天桥下，", "宁姆格福，", "下水道，", "电子信息楼，"};
                static int fnnum = firstName.size();
                static int lnnum = lastName.size();
                static int fdnnum = firstadress.size();
                static int ldnnum = midadress.size();
                static int count = 0;
                static Student tstu;
                char word[64];
                GetWindowTextA(holdtext, word, 64);//从窗口holdtext中获取文字
                string strword = word;
                if (strword.empty()||!isdigit(word[0]))
                {
                    MessageBoxA(hWnd, "请输入数字!", "提示", MB_OK);
                    break;
                }
                count = stoi(word);//若为亚洲字符如中文，日文，韩文，程序会崩溃（sad
                if(count <= 0)
                {
                    MessageBoxA(hWnd, "请输入正整数!", "提示", MB_OK);
                    break;
                }
                for (int i{}; i < count; ++i)
                {
                    tstu.no = 32406100000 + Student::nonum++;
                    ++Student::stunum;
                    tstu.name = firstName[rand() % fnnum] + lastName[rand() % lnnum];
                    tstu.adress = firstadress[rand() % fdnnum] + midadress[rand() % ldnnum] + to_string(rand()%101) + "号";
                    tstu.sex = rand() % 2;
                    tstu.old = rand() % 90;
                    tstu.cscore = rand() % 101;
                    tstu.math = rand() % 101;
                    stuset.push_back(tstu);
                }
                stuidx = -1;
                showmsg(stuidx);
                searchword("");
                RECT trect = { 680, 100, 1180, 330 };
                InvalidateRect(hWnd, &trect, true);//重绘频率直方图
                break;
            }
            //列表框信息映射到右侧的窗口并更新显示
            case ID_STUDENTLISTBOX:
            {
                if (event == LBN_SELCHANGE)
                {
                    int index;
                    index = (int)SendMessageA(hboxlist, LB_GETCURSEL, 0, 0);//获取选中的某个数据的索引
                    char word[256];
                    SendMessageA(hboxlist, LB_GETTEXT, index, (LPARAM)word);//获取hboxlist索引为index的文本
                    static string str, str2;
                    str = word;//可能获取的文本超过256个字符，导致word里面可能没读取到零终止符
                    for (int i{}; i < stuset.size(); ++i)
                    {
                        str2 = to_string(stuset[i].no) + ":" + stuset[i].name;
                        if (str2.find(str.c_str()) != wstring::npos)
                        {
                            stuidx = i;
                            break;
                        }
                        else
                        {
                            stuidx = -1;
                        }
                    }
                    if (stuidx >= 0)
                    {
                        showmsg(stuidx);//映射到右侧其他窗口
                    }
                }
                break;
            }
            //降序排序数学成绩
            case ID_MATHSORT:
            {
                sort(stuset.begin(),stuset.end(), sortmath);
                searchword("");
                break;
            }
            //降序排序C++成绩
            case ID_CPPSORT:
            {
                sort(stuset.begin(),stuset.end(),sortcpp);
                searchword("");
                break;
            }
            //删除选中的学生
            case ID_DESTORY:
            {
                if (stuidx >= 0)
                {
                    //vector的弊端，早知道不用vector了TwT
                    vector<Student> tstu;
                    for (int i{}; i < stuidx; ++i)
                    {
                        tstu.push_back(stuset[i]);
                    }
                    for (int i = stuidx + 1; i < stuset.size(); ++i)
                    {
                        tstu.push_back(stuset[i]);
                    }
                    stuset.clear();
                    for (int i{}; i < tstu.size(); ++i)
                    {
                        stuset.push_back(tstu[i]);
                    }
                    --Student::stunum;
                    searchword("");
                    stuidx = -1;
                    showmsg(stuidx);
                }
                RECT trect = { 680, 100, 1180, 330 };
                InvalidateRect(hWnd, &trect, true);//依旧重绘频率直方图
                break;
            }
            //性别框选择性别
            case ID_MANBTN:
            {
                if (event == BN_CLICKED)
                {
                    if (stuidx >= 0)
                    {
                        stuset[stuidx].sex = 0;
                        //MessageBox(hWnd, L"已更改为男性", L"提示", MB_OK);
                    }
                }
                break;
            }
            case ID_WOMANBTN:
            {
                if (event == BN_CLICKED)
                {
                    if (stuidx >= 0)
                    {
                        stuset[stuidx].sex = 1;
                        //MessageBox(hWnd, L"已更改为女性", L"提示", MB_OK);
                    }
                }
                break;
            }
            //增加学生
            case ID_ADD:
            {
                static bool same;
                static long long tno;
                tno = 32406100000 + Student::nonum;
                Student tstu(tno);
                //这个for循环似乎永远不会进入
                for (const auto& stu : stuset)
                {
                    if (stu.no == tno)
                    {
                        MessageBox(hWnd, L"添加失败！不能有相同的学号", L"提示", MB_OK);
                        same = true;
                        break;
                    }
                }
                if (!same)
                {
                    stuset.push_back(tstu);
                    ++Student::nonum;
                    ++Student::stunum;
                    stuidx = -1;
                    showmsg(stuidx);
                    searchword("");
                }
                RECT trect= { 680, 100, 1180, 330 };
                InvalidateRect(hWnd, &trect, true);//重绘
                break;
            }
            //删除学生
            case ID_CANCEL:
            {
                stuidx = -1;
                showmsg(stuidx);
                break;
            }
            //修改学生信息
            case ID_REVISE:
            {
                if (stuidx >= 0)
                {
                    bool same = false;
                    char word[256];
                    GetWindowTextA(hno, word, 256);//获取信息
                    for (int i{};i<stuset.size();++i)
                    {
                        if (stuset[i].no == stoll(word) && i != stuidx)//防止学号重复
                        {
                            MessageBox(hWnd, L"添加失败！不能与其他同学有相同的学号", L"提示", MB_OK);
                            same = true;
                            break;
                        }
                    }
                    //执行修改
                    if (!same)
                    {
                        stuset[stuidx].no = stoll(word);
                        GetWindowTextA(hname, word, 256);
                        stuset[stuidx].name = word;
                        GetWindowTextA(hold, word, 256);
                        stuset[stuidx].old = stoi(word);
                        GetWindowTextA(hadress, word, 256);
                        stuset[stuidx].adress = word;
                        GetWindowTextA(hmath, word, 256);
                        stuset[stuidx].math = stod(word);
                        GetWindowTextA(hcpp, word, 256);
                        stuset[stuidx].cscore = stod(word);
                        searchword("");
                    }
                }
                RECT trect = { 680, 100, 1180, 330 };
                InvalidateRect(hWnd, &trect, true);//
                break;
            }
            //保存数据
            case ID_COMFIRM:
            {
                savedata();
                break;
            }
            //搜索
            case ID_BUTTONSEARCH:
            {
                GetWindowTextA(hsearchwd, text, 256);
                searchword(text);
                break;
            }
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            static string tstr = "个";
            TextOutA(hdc, 611, 392, tstr.c_str(), tstr.length());
            drowgraph(hdc);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void searchword(const char* text1)
{
    string str;
    SendMessageA(hboxlist, LB_RESETCONTENT, 0, 0);//清空列表框
    for(const auto& word : stuset)
    {
        str = to_string(word.no) + ":" + word.name.c_str();
        if (str.find(text1) != string::npos)
        {
            SendMessageA(hboxlist, LB_ADDSTRING, 0, (LPARAM)str.c_str());//添加文本到列表框
        }
    }
}
//保存数据
void savedata()
{
    if (stuset.empty())
    {
        return;
    }
    ofstream of("studata.txt");
    of << Student::stunum << endl;
    of << Student::nonum << endl;
    for (int i{}; i < stuset.size(); ++i)
    {
        of << stuset[i].no << "|";
        of << stuset[i].name << "|";
        of << stuset[i].cscore << "|";
        of << stuset[i].math << "|";
        of << stuset[i].old << "|";
        of << stuset[i].sex << "|";
        of << stuset[i].adress << endl;
    }
    of.close();
}
//加载数据
void loaddata()
{
    ifstream IF("studata.txt");
    if (is_file_empty("studata.txt"))
    {
        return;
    }
    string line;
    getline(IF, line);
    Student::stunum = stoi(line);
    getline(IF, line);
    Student::nonum = stoi(line);
    Student tstu;
    while (getline(IF, line))
    {
        istringstream iss(line);
        string token;
        getline(iss, token, '|');
        tstu.no = stoll(token);
        getline(iss, token, '|');
        tstu.name = token;
        getline(iss, token, '|');
        tstu.cscore = stod(token);
        getline(iss, token, '|');
        tstu.math = stod(token);
        getline(iss, token, '|');
        tstu.old = stoi(token);
        getline(iss, token, '|');
        tstu.sex = (stoi(token) != 0);
        getline(iss, token);
        tstu.adress = token;
        stuset.push_back(tstu);
    }
    IF.close();
    return;
}
//映射到右侧的窗口
void showmsg(const int& index)
{
    SetWindowText(hno, NULL);
    SetWindowText(hname, NULL);
    SetWindowText(hold, NULL);
    SetWindowText(hadress, NULL);
    SetWindowText(hmath, NULL);
    SetWindowText(hcpp, NULL);
    
    if (index >= 0)
    {
        
        SetWindowTextA(hno, to_string(stuset[index].no).c_str());
        SetWindowTextA(hname, stuset[index].name.c_str());
        SetWindowTextA(hold, to_string(stuset[index].old).c_str());
        SetWindowTextA(hadress, stuset[index].adress.c_str());
        SetWindowTextA(hmath, to_string(stuset[index].math).c_str());
        SetWindowTextA(hcpp, to_string(stuset[index].cscore).c_str());
        if (stuset[stuidx].sex)
        {
            SendMessage(hwomanbtn, BM_SETCHECK, BST_CHECKED, NULL);
            SendMessage(hmanbtn, BM_SETCHECK, BST_UNCHECKED, NULL);
        }
        else
        {
            SendMessage(hmanbtn, BM_SETCHECK, BST_CHECKED, NULL);
            SendMessage(hwomanbtn, BM_SETCHECK, BST_UNCHECKED, NULL);
        }
    }
    return;
}
//用于简单排序的函数
bool sortmath(const Student& a, const Student& b)
{
    return a.math>b.math;
}

bool sortcpp(const Student& a, const Student& b)
{
    return a.cscore > b.cscore;
}
//绘制频率直方图
void drowgraph(HDC& hdc)
{
    if (stuset.size() == 0)
    {
        return;
    }
    static vector<string> gtextlist = {" <60 ", "60-70", "70-80", "80-90", ">= 90"};
    static vector<int> scorecount;
    scorecount.clear();
    scorecount = { 0, 0, 0, 0, 0 };
    int stucount = stuset.size();
    Rectangle(hdc, 680, 100, 1180, 330);

    for (int i{}; i < gtextlist.size(); ++i)
    {
        TextOutA(hdc, 740 + i*83, 310, gtextlist[i].c_str(), gtextlist[i].length());//显示<60,60-70等
    }

    //统计各个分段学生数量
    for (const auto& stu : stuset)
    {
        if ((stu.math + stu.cscore) / 2 < 60)
        {
            scorecount[0] += 1;
        }
        else if((stu.math + stu.cscore) / 2 >= 60 && (stu.math + stu.cscore) / 2 < 70)
        {
            scorecount[1] += 1;
        }
        else if ((stu.math + stu.cscore) / 2 >= 70 && (stu.math + stu.cscore) / 2 < 80)
        {
            scorecount[2] += 1;
        }
        else if ((stu.math + stu.cscore) / 2 >= 80 && (stu.math + stu.cscore) / 2 < 90)
        {
            scorecount[3] += 1;
        }
        else
        {
            scorecount[4] += 1;
        }
    }
    int height = 0;
    //创建画笔与画刷
    HPEN hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN oldpen = (HPEN)SelectObject(hdc, hpen);
    HBRUSH holdbrush = (HBRUSH)SelectObject(hdc, GetStockObject(GRAY_BRUSH));
    for (int i{}; i < gtextlist.size(); ++i)
    {
        
        height = (scorecount[i] / (double)stucount) * 210;
        Rectangle(hdc, 738 + i*83, 310 - height, 778 + i * 83, 310);
        
    }
    
    DeleteObject(SelectObject(hdc, oldpen));//释放画笔并选回原画笔
    SelectObject(hdc, holdbrush);//选择原来的画刷
}

//判断文件是否为空
bool is_file_empty(const std::string& filename) 
{
    std::ifstream file(filename);
    return file.peek() == std::ifstream::traits_type::eof();
}
