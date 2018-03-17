package main

import (
	"bufio"
	"fmt"
	"net"
	"time"
)

func main() {
	var tcpAddr *net.TCPAddr

	go f()
	tcpAddr, _ = net.ResolveTCPAddr("tcp", "127.0.0.1:9999")
	tcpListener, _ := net.ListenTCP("tcp", tcpAddr)
	defer tcpListener.Close()
	fmt.Println(tcpAddr)
	for {
		tcpConn, err := tcpListener.AcceptTCP()
		if err != nil {
			fmt.Println("Accept error:", err)
			continue
		}
		fmt.Println("A client connected: " + tcpConn.RemoteAddr().String())
		go handler(tcpConn)
	}

	fmt.Println("Finish")
}

var ch1 chan int = make(chan int, 1000*10000)

func f() {
	for {
		time.Sleep(1e9)
		fmt.Println("========> the number is:", len(ch1))
	}
}

func handler(conn *net.TCPConn) {
	ipStr := conn.RemoteAddr().String()
	ch1 <- 1
	defer func() {
		fmt.Println("disconnected: " + ipStr)
		conn.Close()
		_ = <-ch1
	}()

	reader := bufio.NewReader(conn)

	for {
		msg, err := reader.ReadString('\n')
		if err != nil {
			fmt.Println("Read error:", err)
			return
		}
		fmt.Println("recv:", string(msg))
		msg = time.Now().String() + "\n"
		b := []byte(msg)
		conn.Write(b)
	}
}
