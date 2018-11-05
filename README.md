# MAL-analyzer
This is a program that analyzes data from MyAnimeList. The data was pulled with a separate program using libcurl and multithreading. This program takes in the following arguments:  

dendrogram1 x: creates a dendrogram of the top x most popular anime, displays it, and saves an image  
similar x: lists the correlation (r) between some anime and the anime lexicographically equal to or after x  
lsize x: sets the list size to x  
cluster1 x y: uses simulated annealing to form a group of similar anime of size y from the top x anime  
group1 x y: uses simulated annealing to form y groups of similar anime from the top x anime  
plotr x: plots the anime ratings of user x vs the global ratings of the same anime and calculates r  
plotu x y: plots the anime ratings of two users against each other and calculates r  
like x y: estimates how much user x will like anime y  
liker x: plots the estimated ratings of anime of user x vs that user's actual ratings to determine how accurate the rating predictor is  
liken x y: estimates how much user x will like the top y most popular anime  
rate x: determines the average rating for anime x among users in the dataset and the global rating for anime x  
raten x: performs the rate command for the top x most popular anime and displays a list  
rates x: lists users who have high rating affinities with user x  
ratet x: changes the threshold for how many anime two users must have in common to be displayed on the list

example dendrogram that clusters the top 100 most popular anime (x axis is labelled with r values of the scores)
![alt text](https://github.com/kliu31415/MAL-analyzer/blob/master/dendrogram.png?raw=true)
