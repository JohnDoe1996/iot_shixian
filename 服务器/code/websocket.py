# coding:utf-8

import tornado.web
import tornado.ioloop
import tornado.httpserver
import tornado.options
import os
import datetime
import json

from mongodb import database

from tornado.web import RequestHandler
from tornado.options import define, options
from tornado.websocket import WebSocketHandler

define("port", default=8888, type=int)


stateAll = [0,0,0,0,0,0,0]
db = database()

class API(RequestHandler):
    def get(self):
        choose = self.get_argument("choose","")
        limit = self.get_argument("limit","")
        (tData,hData,lData,vData) = db.findAll(int(limit))
        dataDict = {}
        if choose == "th":
            dataDict["tData"] = tData[::-1]
            dataDict["hData"] = hData[::-1]
        elif choose == "lv":
            dataDict["lData"] = lData[::-1]
            dataDict["vData"] = int(vData[0]*100/(vData[0]+vData[1]))
        else:
            dataDict["result"] = "None"
        self.write(json.dumps(dataDict))


class IndexHandler(RequestHandler):
    def get(self):
        self.render("index.html")

class ChatHandler(WebSocketHandler):

    users = set()  # 用来存放在线用户的容器

    def open(self):
        self.users.add(self)  # 建立连接后添加用户到容器中
        print("[%s]-[%s]-connect" % (self.request.remote_ip, datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")))
        for u in self.users:  # 向已在线用户发送消息
            u.write_message(u"[%s]-[%s]-connect" % (self.request.remote_ip, datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")))

    def on_message(self, message):
        print(message)
        sendData = ""
        if message=="check_all": # 返回所有数据
            sendData = "all " + " ".join(map(str,stateAll))

        if message.split(" ")[0]=="set_all": # 设置所有数据
            for i in range(len(stateAll)):
                stateAll[i] = message.split(" ")[i+1]
            db.addOne(stateAll[-4],stateAll[-3],stateAll[-2],stateAll[-1]) # 传感器数据保存到数据库
            sendData = "all " + " ".join(map(str, stateAll))

        if message.split(" ")[0]=="set_switch": # 设置开关的数据
            conLit = []
            for i in range(3):
                conLit.append( message.split(" ")[i+1] )
                # stateAll[i] = message.split(" ")[i+1]
            # sendData = "all " + " ".join(map(str, stateAll))
            sendData = "con " + " ".join(map(str, conLit))

        if message.split(" ")[0]=="set_thlv": # 设置传感器的数据
            for i in range(1,4):
                stateAll[-i] = message.split(" ")[-i]
            db.addOne(stateAll[-4],stateAll[-3],stateAll[-2],stateAll[-1]) # 传感器数据保存到数据库
            sendData = "all " + " ".join(map(str, stateAll))
        print(sendData)
        for u in self.users:  # 向在线用户广播消息
            u.write_message(sendData)

    def on_close(self):
        self.users.remove(self) # 用户关闭连接后从容器中移除用户
        print("[%s]-[%s]-leave" % (self.request.remote_ip, datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")))
        for u in self.users:
            u.write_message(u"[%s]-[%s]-leave" % (self.request.remote_ip, datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")))

    def check_origin(self, origin):
        return True  # 允许WebSocket的跨域请求

if __name__ == '__main__':
    tornado.options.parse_command_line()
    app = tornado.web.Application([
            (r"/", IndexHandler),
            (r"/chat", ChatHandler),
            (r"/api", API),
        ],
        static_path=os.path.join(os.path.dirname(__file__), "static"),
        template_path=os.path.join(os.path.dirname(__file__), "templates"),
        debug=False
        )
    http_server = tornado.httpserver.HTTPServer(app)
    http_server.listen(options.port)
    tornado.ioloop.IOLoop.current().start()