# Miscellany Files

Here we will get through what are the contents of the directory **`path/to/includes/miscellany`** and what purpose may it serve for any future additions.

## Table of Contents

1. [Miscellany Files](#miscellany-files)
    1. [Table of Contents](#table-of-contents)
    2. [Main Purpose](#main-purpose)
    3. [Usage](#usage)

## Main Purpose

This directory is originally built to house the scripts and extra bits that might be useful (albeit to write another component or while running a certain model simulation). For the time being, it houses only the [scripts](../../includes/miscellany/abm.py) that creates the coefficients for *Adams-Bashforth/Adams-Bashforth-Moulton* (Predictor/Predictor-Corrector) solvers.

## Usage

If there is a new script (to bootstrap a new component for instance) the said script would be stored here for later debugging and analyses. If you happen to have any code that would be useful with your own model (e.g. a python script that plots some data, a python script that exposes the library to a python interface, and so on.), you might add them here. Feel free to store all the data files (for instance take a look at the [`.txt`](../../includes/miscellany/ABM-Coefs/abm_coefs_order4.txt) files in the `path/to/includes/miscellany/ABM-coefs/abm_coefs_order*.txt`).
