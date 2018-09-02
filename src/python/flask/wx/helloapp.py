#!/usr/bin/env python
#coding:utf-8

from flask import Flask
from flask import request
from WXBizMsgCrypt import WXBizMsgCrypt
import xml.etree.cElementTree as et
import time
from flask import Flask,g,request,make_response
import hashlib
import xml.etree.ElementTree as ET
import daemon_init



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
        rec = request.stream.read()
        xml_rec = ET.fromstring(rec)
        tou = xml_rec.find('ToUserName').text
        fromu = xml_rec.find('FromUserName').text
        content = xml_rec.find('Content').text
        xml_rep = "<xml><ToUserName><![CDATA[%s]]></ToUserName><FromUserName><![CDATA[%s]]></FromUserName><CreateTime>%s</CreateTime><MsgType><![CDATA[text]]></MsgType><Content><![CDATA[%s]]></Content><FuncFlag>0</FuncFlag></xml>"
        response = make_response(xml_rep % (fromu,tou,str(int(time.time())), content))
        response.content_type='application/xml'
        return response
    return "ABCD"

if __name__ == "__main__":
    daemon_init.daemon_init()
    app.run(host = "0.0.0.0", port = 80)

