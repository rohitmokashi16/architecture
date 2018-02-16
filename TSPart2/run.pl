#!/usr/bin/perl
#
# Run script by typing: run.pl
# The run.pl file should have execute permissions.
#
# This script runs on student.  If you run it on other machines,
# you may need to make changes to it to get it to work.
#
# The script assumes that your program was written in Java
# and the code is in an Assemble.jar file and thus executes
# the command: java -jar Assemble.jar <input file>
# You will need to change the script, if this isn't the how to execute
# your code.
#
# If your program fails on an input file, then the input file, the
# output produced by your program on that input file, and a file 
# containing the differences between the output and the expected output
# are placed in your Tests directory.  If your program doesn't produce
# an output file for a particular input, then the input file alone
# is placed the Tests directory.  If your program passes all tests, then
# the Tests directory should be empty after running the script.
#

@canfiles = ( "intUnit1",  "intUnit2",  "intUnit3",  "intUnit4",  
              "intUnit5",  "intUnit6",  "intUnit7",  "intUnit8", 
              "intUnit9");

$candir = "/u/css/classes/5483/161/TSPart2/Inputs/";
#$candir = "Inputs/";

$dir = "Tests/";

if (! -e $dir)
{
   print "need to create a Tests directory first\n";
   exit();
}
system "rm -f Tests/*";                                                                                

$pass = 0;

for ($i = 0; $i <= $#canfiles; $i++){
   $prefix = $canfiles[$i];
   $input = $prefix.".hex";
   $output = $prefix.".out";
   $caninput = $candir.$prefix.".hex";
   $canoutput = $candir.$prefix.".out";
   system "cp $caninput .";
   print "Testing $input. \n";
   
   
   system "./tomasulo $input -D > $output";

   $problemFile = $prefix . ".problems";
   if (-e $output) {
       system "diff -B -E -b $output $canoutput > $problemFile";
       $problemFileSize = -s $problemFile;
   }

   if (!(-e $output)) {
         #didn't create an output file, copy input file to Tests directory
         print " No output file. Failed.\n";
         system "mv $input Tests/";
   } elsif ($problemFileSize > 0){
         print " Diffs in output. Failed. Moving input, your output, and diffs to Tests dir.\n";
         system "mv $input Tests/";
         system "mv $output Tests/";
         system "mv $problemFile Tests/";
   } else {
         system "rm -rf $output $input $problemFile";
         print " Passed.\n";
         $pass = $pass + 1;
   }
}

$total = $#canfiles + 1;
print "\n$pass out of $total passed.\n";
if ($pass != $total) {
   print "See Tests directory for failed tests\n";
}


