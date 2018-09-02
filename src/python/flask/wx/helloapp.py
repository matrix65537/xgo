#!/usr/bin/env python
#coding:utf-8

from flask import Flask
from flask import request
from WXBizMsgCrypt import WXBizMsgCrypt

app = Flask(__name__)

@app.route("/")
def hello():
    print request.args
    print "-" * 80
    for k, v in request.args.items():
        print k, ":", v
    print "-" * 80
    encodingAESKey = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFG"
    to_xml = """ <xml><ToUserName><![CDATA[oia2TjjewbmiOUlr6X-1crbLOvLw]]></ToUserName><FromUserName><![CDATA[gh_7f083739789a]]></FromUserName><CreateTime>1407743423</CreateTime><MsgType>  <![CDATA[video]]></MsgType><Video><MediaId><![CDATA[eYJ1MbwPRJtOvIEabaxHs7TX2D-HV71s79GUxqdUkjm6Gs2Ed1KF3ulAOA9H1xG0]]></MediaId><Title><![CDATA[testCallBackReplyVideo]]></Title><Descript  ion><![CDATA[testCallBackReplyVideo]]></Description></Video></xml>"""

    token = request.args.get("token", "token")
    nonce = request.args.get("nonce", "nonce")
    appid =  request.args.get("appid", "appid")

    encryp_test = WXBizMsgCrypt(token,encodingAESKey,appid)
    ret,encrypt_xml = encryp_test.EncryptMsg(to_xml,nonce)
    return encrypt_xml

if __name__ == "__main__":
    app.run()

