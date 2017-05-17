//Go语言面向对象多态
package main

import (
	"fmt"
)

type Animal interface {
	eat()
}

type Cat struct {
}

func NewCat() *Cat {
	return &Cat{}
}

func (cat *Cat) eat() {
	fmt.Println("cat eat")
}

type Dog struct {
}

func NewDog() *Dog {
	return &Dog{}
}

func (dog *Dog) eat() {
	fmt.Println("dog eat")
}

func main() {
	var animal Animal
	var cat *Cat
	var dog *Dog

	cat = NewCat()
	cat.eat()

	dog = NewDog()
	dog.eat()

	animal = NewCat()
	animal.eat()

	animal = NewDog()
	animal.eat()
}
