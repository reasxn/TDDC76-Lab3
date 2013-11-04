
# Diagnosmeddelanden frÂn kompilatorn (g++) filtreras av gccfilter.
CCC = gccfilter -c -a g++

# Kompilatorflaggor, l‰gg till '-g' om kompilering fˆr avlusning ska gˆras.
CCFLAGS += -std=c++11 -Wpedantic -Wall -Wextra -Werror

# L‰nkflaggor - Fix fˆr att lˆsa lokala problem med C++ l‰nkbibliotek.
LDFLAGS  += -L/sw/gcc-$(GCC4_V)/lib -static-libstdc++

# Objektkodsmoduler som ingÂr i den kompletta kalkylatorn.
OBJECTS = Calculator.o kalkylator.o Expression.o Expression_Tree.o Variable_Table.o
# HuvudmÂl - skapas med kommandot 'make' eller 'make kalkylator'.
kalkylator: $(OBJECTS) makefile
	$(CCC) $(CCFLAGS) $(LDFLAGS) -o kalkylator $(OBJECTS)

# DelmÂl (flaggan -c avbryter innan l‰nkning, objektkodsfil erhÂlls)
Calculator: Calculator.cc Calculator.h
	$(CCC)  $(CCFLAGS) -c Calculator.cc
	
Variable_Table.o: Variable_Table.h Variable_Table.cc
	$(CCC)  $(CCFLAGS) -c Variable_Table.cc
	
Expression.o: Expression.h Expression.cc
	$(CCC) $(CCFLAGS) -c Expression.cc

kalkylator.o: kalkylator.cc
	$(CCC) $(CCFLAGS) -c kalkylator.cc

Expression_Tree.o: Expression_Tree.h Expression_Tree.cc
	$(CCC) $(CCFLAGS) -c Expression_Tree.cc
# 'make clean' tar bort objektkodsfiler och 'core' (minnesdump).
clean:
	@ \rm -rf *.o *.gch core

# 'make zap' tar ‰ven bort det kˆrbara programmet och reservkopior (filer
# som slutar med tecknet '~').
zap: clean
	@ \rm -rf kalkylator *~

# Se upp vid eventuell ‰ndring, '*' fÂr absolut inte finnas fˆr sig!!!
#
# '@' medfˆr att kommandot inte skrivs ut pÂ sk‰rmen dÂ det utfˆrs av make.
# 'rm' ‰r ett alias fˆr 'rm -i' pÂ IDA:s system, '\rm' inneb‰r att "original-
# versionen", utan flaggan '-i', anv‰nds. 
