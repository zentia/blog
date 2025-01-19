---
title: Finite State Machine
mathjax: true
date: 2019-09-03 12:27:23
tags:
categories:
---
# Description
This is Deterministic Finite State Machine framework based on chapter 3.1 of Game Programming Gems 1 by Eric Dybsend. There are two classes and two enums.Include them in your project and follow the explanations to get the FSM working properly.There's also a complete example script at the end of this page.
# Components
- **Transition enum**:This enum contains the labels to the transition that can be fired by the system.Don't change the first label,NullTransition,as the FSMSystem class uses it.
- **StateID enum**:This is the ID of the states the game may have.You could use references to the real State's classes but using enums makes the system less susceptible to have code having access to objects it is not supposed to.All the state's ids should be placed here.Don't change the first label,NullStateID,as the FSMSystem class uses it.

