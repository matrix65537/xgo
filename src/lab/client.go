package main

import (
	"bufio"
	"fmt"
	"net"
	"time"
)

var quit chan bool

func main() {
	var tcpAddr *net.TCPAddr

	for i := 0; i < 1000; i++ {
		tcpAddr, _ = net.ResolveTCPAddr("tcp", "127.0.0.1:9999")
		conn, _ := net.DialTCP("tcp", nil, tcpAddr)
		defer conn.Close()

		fmt.Println("connected!")

		go handler(conn)

		b := []byte("ABCD\n")
		conn.Write(b)

	}

	<-quit

	fmt.Println("Finish")
}

func handler(conn *net.TCPConn) {
	reader := bufio.NewReader(conn)
	for {
		msg, err := reader.ReadString('\n')
		fmt.Println(msg)
		if err != nil {
			quit <- true
			break
		}
		time.Sleep(time.Second)
		b := []byte(msg)
		conn.Write(b)
	}
}
