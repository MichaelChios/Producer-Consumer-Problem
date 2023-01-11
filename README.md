# Producer-Consumer-problem


Ζητείται να γραφεί πρόγραμμα σε γλώσσα C που επιλύει
αποτελεσματικά και επιδεικνύει τη λύση στο πρόβλημα του
παραγωγού-καταναλωτή (Producer - Consumer problem).

Πιο αναλυτικά για το πρόβλημα εδώ:
https://en.wikipedia.org/wiki/Producer%E2%80%93consumer_problem

Ο ρόλος των παραγωγών είναι να δημιουργούν δεδομένα που
τοποθετούν σε κοινό χώρο μνήμης με συγκεκριμένη χωρητικότητα.

Οι καταναλωτές πρέπει να χρησιμοποιούν όλα τα δεδομένα που
έχουν δημιουργηθεί.

Ζητείται να υλοποιηθεί λύση μου δημιουργεί ένα ή περισσότερα
νήματα (ή διεργασίες) για τους παραγωγούς και ένα ή περισσότερα
νήματα (ή διεργασίες) για τους καταναλωτές.

Ακολουθεί ένας αλγόριθμος επίλυσης με χρήση σημαφόρων.

Initialization of semaphores
----------------------------
mutex = 1 // mutual exclusion
full = 0 // initially, all slots are empty, so full slots are 0
empty = n // all slots are empty, n=size of buffer

Solution for Producer
---------------------
while(true) {

// produce an item

wait(empty);
wait(mutex);

// place item in buffer

signal(mutex);
signal(full);
}

Solution for Consumer
---------------------
while(true) {

wait(full);
wait(mutex);

// remove item from buffer

signal(mutex);
signal(empty);

// consume item
}
