import csv
from py_expression_eval import Parser
parser = Parser()

def formula_to_steps(maxSteps, motors, minX, maxX, minY, maxY, formulaString):
    formula = parser.parse(formulaString)
    stepDifference = maxSteps * 2
    yDifference = maxY - minY
    xDifference = maxX - minX
    xDifferenceMotor = float(xDifference) / float(motors)
    stepsPerY = float(stepDifference) / float(yDifference)
    print str(stepsPerY) + " steps"
    print "1 y = " + str(stepsPerY) + " steps"
    print xDifferenceMotor
    with open('graph.csv', 'wb') as csvfile:
        writer = csv.writer(csvfile)
        for i in range(motors):
            x = i * xDifferenceMotor
            print str(i) + " > x = " + str(x)
            y = formula.evaluate({'x': x})
            steps = y * stepsPerY
            writer.writerow([i, steps])

formula_to_steps(1000, 30, 0, 360, -1, 1, "sin(x*PI/180)")
