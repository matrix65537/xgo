package algorithm

import (
	_ "fmt"
	"testing"
)

func TestStackPush(t *testing.T) {
	stack := NewStack(4)
	stack.Push(1)
	stack.Push(2)
	stack.Push(3)
	stack.Push(4)
	x, _ := stack.Top()
	if x != 4 {
		t.Error("stack top not 4")
	}

	a, _ := stack.Pop()
	b, _ := stack.Pop()
	c, _ := stack.Pop()
	d, _ := stack.Pop()
	if (a != 4) || (b != 3) || (c != 2) || (d != 1) {
		t.Error("stack pop error")
	}
}

func TestStackFull(t *testing.T) {
	stack := NewStack(2)
	stack.Push(1)
	stack.Push(2)
	err := stack.Push(3)
	if err != ErrStackFull {
		t.Error("stack full error")
	}
	a, _ := stack.Pop()
	b, _ := stack.Pop()
	if (a != 2) || (b != 1) {
		t.Error("stack pop error")
	}
}

func TestStackEmpty(t *testing.T) {
	stack := NewStack(2)
	stack.Push(1)

	if stack.Empty() {
		t.Error("stack not empty")
	}
	stack.Pop()
	if !stack.Empty() {
		t.Error("stack is empty")
	}

	_, err := stack.Pop()
	if err != ErrStackEmpty {
		t.Error("stack should be empty")
	}
}
