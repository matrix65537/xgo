#!/usr/bin/env python
#coding:utf-8

from flask import Flask
from flask import request
from WXBizMsgCrypt import WXBizMsgCrypt
import xml.etree.cElementTree as et


app = Flask(__name__)

i = 0

@app.route("/", methods = ['GET', 'POST'])
def hello():
    global i
    if request.method == 'GET':
        print request.args
        print "-" * 80
        for k, v in request.args.items():
            print k, ":", v
        print "-" * 80
        encodingAESKey = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFG"
        to_xml = """ <xml><ToUserName><![CDATA[oia2TjjewbmiOUlr6X-1crbLOvLw]]></ToUserName><FromUserName><![CDATA[gh_7f083739789a]]></FromUserName><CreateTime>1407743423</CreateTime><MsgType>  <![CDATA[video]]></MsgType><Video><MediaId><![CDATA[eYJ1MbwPRJtOvIEabaxHs7TX2D-HV71s79GUxqdUkjm6Gs2Ed1KF3ulAOA9H1xG0]]></MediaId><Title><![CDATA[testCallBackReplyVideo]]></Title><Descript  ion><![CDATA[testCallBackReplyVideo]]></Description></Video></xml>"""

        signature = request.args.get("signature", "signature")
        timestamp = request.args.get("timestamp", "timestamp")
        nonce =  request.args.get("nonce", "nonce")
        echostr =  request.args.get("echostr", "echostr")

        print echostr

        return echostr
    else:
        i += 1
        print "This is a post [%d]" %(i)
        return "ABCD"


    #encryp_test = WXBizMsgCrypt(token,encodingAESKey,appid)
    #ret,encrypt_xml = encryp_test.EncryptMsg(to_xml,nonce)
    #return encrypt_xml

if __name__ == "__main__":
    app.run(host = "0.0.0.0", port = 80)

