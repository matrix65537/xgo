//Go语言实现栈
package algorithm

import (
	"errors"
)

var ErrStackEmpty = errors.New("Stack is empty")
var ErrStackFull = errors.New("Stack is full")

type Stack struct {
	maxsize int //the maxsize for stack
	datas   []int
	index   int //sp pointer to the empty slot
}

func NewStack(maxsize int) *Stack {
	return &Stack{datas: make([]int, maxsize), index: 0, maxsize: maxsize}
}

func (stack *Stack) Push(x int) (err error) {
	if stack.index >= stack.maxsize {
		return ErrStackFull
	} else if stack.index == len(stack.datas) {
		stack.datas = append(stack.datas, x)
	} else {
		stack.datas[stack.index] = x
	}
	stack.index += 1
	return nil
}

func (stack *Stack) Pop() (x int, err error) {
	if stack.index == 0 {
		err = ErrStackEmpty
	} else {
		stack.index -= 1
		x = stack.datas[stack.index]
	}
	return
}

func (stack *Stack) Top() (x int, err error) {
	if stack.index == 0 {
		err = ErrStackEmpty
	} else {
		x = stack.datas[stack.index-1]
	}
	return
}

func (stack *Stack) Empty() (b bool) {
	return stack.index == 0
}
