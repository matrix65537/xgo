//TODO: 管道的流处理，类似linux命令行的管道处理，数据从一个地方处理完成再流到下一个地方处理
package main

import (
	"fmt"
)

//数据节点定义
type DataNode struct {
	X int
	Y int
	Z int
}

func NewDataNode(x, y, z int) *DataNode {
	return &DataNode(x, y, z)
}

type PipeNode struct {
	datanode *DataNode
	handler  func(datanode *DataNode) *DataNode
	next     chan *DataNode
}

func NewPipeNode(datanode *DataNode, handler func(datanode *DataNode) *DataNode, next chan *DataNode) *PipeNode {
	return &PipeNode(datanode, handler, next)
}

//首先创建3个不同的处理函数，然后用管道把它们串联起来，最后使之像多米诺骨牌一样依次流动处理

func handlerA(datanode *DataNode) {
	fmt.Println("handlerA", datanode)
	datanode.X += 1
}

func handlerB(datanode *DataNode) {
	fmt.Println("handlerB", datanode)
	datanode.Y += 1
}

func handlerC(datanode *DataNode) {
	fmt.Println("handlerC", datanode)
	datanode.Z += 1
}

func Action([]*PipNode)

func main() {
	//创建数据节点
	dataNodeA := NewDataNode(1, 2, 3)
	dataNodeB := NewDataNode(4, 5, 6)

	//创建2个管道，3个处理节点，并把它们连接起来
	//Process1 -> Process2 -> Process3
	ch1 := make(chan *DataNode)
	ch2 := make(chan *DataNode)

	pipnode1 := NewPipeNode(dataNodeA, handlerA, ch1)
	pipnode2 := NewPipeNode(dataNodeA, handlerB, ch2)
	pipnode3 := NewPipeNode(dataNodeA, handlerC, nil)

	fmt.Println("Finish")
}
