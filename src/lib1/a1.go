package lib1

import (
	"fmt"
	_ "lib2"
)

func init() {
	fmt.Println("lib1 init1")
}

func init() {
	fmt.Println("lib1 init2")
}
