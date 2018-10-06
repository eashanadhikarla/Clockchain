# Clockchain
The purpose of this assignment is to get you thinking about what it means for something to happen "before" something else. You will explore this notion through the journey of this project.

The main idea in a BlockChain is that the provenance of a piece of information becomes increasingly difficult to forge over time. That is, every time data (a "block") is added to the chain, that data reinforces the fact that the data that precedes it in the chain is, indeed, present and immutable.

In this project, we will make "clockchain".  The idea behind "clockchain" is that a server (the "clockchain server") will periodically insert timestamps into the chain.  A user can also add blocks of data to the chain.  If we trust that the timestamps are, indeed, inserted at the time they claim to have been inserted, then we can look at a block of data, and the next clock, and conclude that the block of data had to have been inserted before the clock's time.

Clockchain could be a useful idea for an inventor: she could use it to prove that she had an idea before someone else, by showing that her idea is in the clockchain before someone else's idea.  In that case, one might only insert a hash of the idea into the clockchain (because the idea might need to be kept secret), but that's fine... the inventor could still prove that she had the idea by presenting the idea and showing that it generates the hash value that is in the chain.

There are several steps to this project.
