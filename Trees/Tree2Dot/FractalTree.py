import turtle


# Fractal tree
def draw_branch(t:turtle.Pen, length):
    if length > 5:
        # Pen size
        sz = int(length / 20)
        if sz < 1:
            sz = 1
        t.pensize(sz)

        # Main branch
        t.forward(length)

        # turn right by 30 degrees
        t.right(30)
        # right sub-tree, the length of its main branch is (length - 15) pixels
        draw_branch(t, length - 15)

        # turn left by 60 degrees
        t.left(60)
        # left sub-tree,  the length of its branch is (length - 15) pixels
        draw_branch(t, length - 15)

        # turn right by 30 degrees
        t.right(30)
        # go back to the origin position
        t.backward(length)


def draw():
    # Default direction:  --->
    t = turtle.Pen()

    # Direction: Up
    t.left(90)
    t.penup()
    t.backward(260)
    t.pendown()    
    t.shape("turtle")
    t.speed(6)
    
    colors = ['red', 'green']
    for color in colors:
        t.pencolor(color)
        draw_branch(t, 120)
    
    #t.hideturtle()

    screen = turtle.Screen()
    #canvas = screen.getcanvas()
    #canvas.postscript(file='FractalTree.eps')   
    screen.exitonclick()


if __name__ == '__main__':
    draw()
        
    	

