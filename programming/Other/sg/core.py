import win32gui
import time
import requests
import base64
import socket
from PIL import ImageGrab

from PyQt5.QtCore import QThread, pyqtSignal

TOKEN_CACHE = None
# 可自定义的参数
OCR_KEY_WORD = '掉线'
SCAN_INTERVAL = 10
HWND_TITLE_CONTAIN = 'QQ'
#
EMAIL_SEND_ACCOUNT = 'x_jwei@sina.cn'
EMAIL_SEND_PASSWORD = "xjw13663429459"
EMAIL_RECV_ACCOUNT = ['x_jwei@qq.com', '382324131@qq.com']

class Core(QThread):
    sinMsg = pyqtSignal(str)

    def __init__(self, *args, **kwargs):
        super().__init__()
        self.running = True
        global OCR_KEY_WORD, SCAN_INTERVAL, HWND_TITLE_CONTAIN, EMAIL_RECV_ACCOUNT
        self.ui = args[0]
        HWND_TITLE_CONTAIN = args[1]
        OCR_KEY_WORD = args[2]
        SCAN_INTERVAL = int(args[3])
        EMAIL_RECV_ACCOUNT = args[4].split('|')

        print(args)
        self.hostname = socket.gethostname()
        self.ui.sinExit.connect(self.slot_exit)
        self.ts = time.time()

    def slot_exit(self):
        self.running = False
        # self.wait()

    def __del__(self):
        self.slot_exit()
        self.sinMsg.emit('Release:2019/12/1')

    def print_wrap(some_thing):
        def decorator(func):
            def proxy(*args, **kwargs):
                ret = func(*args, **kwargs)
                if isinstance(some_thing, str):
                    args[0].sinMsg.emit('[{0}] {1}'.format(
                        str(time.strftime("%H:%M:%S", time.localtime())),
                        some_thing))
                return ret

            return proxy

        return decorator

    def run(self):
        while self.running:
            try:
                print('=' * 25)
                while not self.find_hwnd():
                    if not self.running:
                        raise RuntimeError('break')
                    time.sleep(2)
                if time.time() >= self.ts:
                    self.judge()
                time.sleep(SCAN_INTERVAL)
            except Exception as e:
                self.sinMsg.emit(str(e))
                print(e)


    def get_token(self):
        global TOKEN_CACHE
        if TOKEN_CACHE is not None:
            return TOKEN_CACHE

        API_KEY = 'WlZ4vi1jtWdBlWX1NduwdOu2'
        SECRET_KEY = 'ww3jvtC0dX4rEH9tWLh8VNHz1N7rMMW2'

        host = 'https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id={0}&client_secret={1}'.format(
            API_KEY, SECRET_KEY)
        response = requests.get(host, timeout=5)
        if response:
            ret = response.json()
            TOKEN_CACHE = ret.get('access_token', None)
            return TOKEN_CACHE
        return None

    def do_ocr(self, key_word, image_bytes=None):
        request_url = "https://aip.baidubce.com/rest/2.0/ocr/v1/general_basic"

        img = base64.b64encode(image_bytes)

        params = {"image": img}
        access_token = self.get_token()
        request_url = request_url + "?access_token=" + access_token
        headers = {'content-type': 'application/x-www-form-urlencoded'}
        response = requests.post(request_url, data=params, headers=headers)
        if response:
            print(response.json())
            words_result = response.json().get('words_result', None)
            if words_result:
                for item in words_result:
                    if item['words'].find(key_word) >= 0:
                        self.sinMsg.emit('找到关键字:' + key_word)
                        return True
        return False

    def send_email(self, subject, content):
        import zmail
        mail = {
            'subject': subject,
            'content_text': content,
            'to': 'x_jwei@qq.com',
            'from': 'x_jwei@sina.cn'
        }

        # 使用你的邮件账户名和密码登录服务器
        server = zmail.server(EMAIL_SEND_ACCOUNT, EMAIL_SEND_PASSWORD)
        # 发送邮件
        server.send_mail(EMAIL_RECV_ACCOUNT, mail)
        self.ts = time.time() + 60*20

    def getWindowPosition(self, window):
        # shell = win32com.client.Dispatch("WScript.Shell")
        # shell.SendKeys('%')
        # win32gui.SetForegroundWindow(window)
        pos = win32gui.GetWindowRect(window)
        self.sinMsg.emit("窗体-{0}:".format(HWND_TITLE_CONTAIN) + str(pos))
        return pos[0] >= 0

    @print_wrap('find_hwnd...')
    def find_hwnd(self):
        titles = set()

        def enum_hwnd(hwnd, mouse):
            if win32gui.IsWindow(hwnd) and win32gui.IsWindowVisible(hwnd):
                titles.add(win32gui.GetWindowText(hwnd))

        win32gui.EnumWindows(enum_hwnd, 0)
        lt = [t for t in titles if t]
        for t in lt:
            print(t)
            if t.find(HWND_TITLE_CONTAIN) >= 0:
                hwnd = win32gui.FindWindow(None, t)
                if hwnd is not None:
                    return self.getWindowPosition(hwnd)

    def getScreenImage(self):
        raw_img = ImageGrab.grab()
        import io
        img_buf = io.BytesIO()
        raw_img.save(img_buf, format='jpeg')
        return img_buf.getbuffer()

    def judge(self):
        image = self.getScreenImage()
        is_down = self.do_ocr(OCR_KEY_WORD, image)
        if is_down:
            self.sinMsg.emit('[judge success]')
            self.send_email('捕获应用[{1}]的通知，来自主机[{0}]'.format(self.hostname, HWND_TITLE_CONTAIN), OCR_KEY_WORD)
        else:
            self.sinMsg.emit('[judge fail]')
