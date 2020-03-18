# Temperature control simulation

Build: *make*

Usage: *./tools/plot.py type [kP kI kD ref] iters*

    type - {pid|tres} - Algorithm type: PID or Treshold
    kP - Proportional (P) component (PID only)
    kI - Integrative (I) component (PID only)
    kD - Derivative (D) component (PID only)
    ref - Reference temperature value
    iters - Max number of PID iterations

Python dependencies: *matplotlib*
