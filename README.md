# TVCTL
Tool for keeping track of watched TV series and episodes
  
## Building and running
You will need `libcurl` to build and use TVCTL.  
Build using these commands:  
    $ mkdir -p obj
    $ make
    
  
Executable will be created in your current directory. Run it with: `./tvctl`.

  
## Usage  
    ./tvctl [command] [parameter]
      
Available commands are:
* `add "[series_name]"`
* `remove "[series_name]"`
* `see "[series_name]" [SxxEyy] `
* `new "[series_name]" [SxxEyy]`
* `check (info) (seen)`
* `update`
   
     

### `add` command:  
Adds new TV series to database.
Example:
  
    $ tvctl add "Firefly"
    Searching for Firefly...
    Updating database...
    Parsing episodes of Firefly...
    Adding new episode 'The Train Job'...
    ...
    Saving changes...

_NOTE:_ When multiple TV series are found, TVCTL will ask to choose correct one:

    $ tvctl add "Stargate"
    Searching for Stargate...
    1) Stargate Universe 
    2) Stargate: Infinity 
    3) Stargate SG-1 
    4) Stargate Atlantis 
    
    Choose from 1-4: ▉  
  
   
     

### `remove` command:  
Removes TV series from database.
Example:

    $ tvctl remove "Game of Thrones"
    Removing Game of Thrones...
    Series 'Game of Thrones' removed.
    

   
   

### `see` command:  
Marks an episode as seen.
Example:

    $ tvctl see "Game of Thrones" S01E09
    Marking Baelor as seen.
    Saving changes...
    

_NOTE:_ You can also specify season or episode as * (asterisk):
`tvctl see "Game of Thrones" "S01E*"`
   
_NOTE2:_ Your shell will probably try to expand asterisk! To prevent that put an argument in quotes.

   
   

### `new` command:
Marks an episode as unseen.
Command syntax is exacly the same as `see` command.

   
   

### `check` command:
Prints (by default) all unseen episodes.
`check` command accepts two optional arguments:
* info - prints extra info about episodes
* seen - prints seen episodes.

_NOTE:_ You can [use `grep` command](https://www.gnu.org/software/grep/manual/grep.html) to print only specific series or/and episodes: `tvctl check | grep "Doctor Who 2005 S01"`

   
   

### `update` command:
Forces database update. Command is automatically invoked before any other command, except `check` command!

## THETVDB
TVCTL uses [TheTVDB](http://thetvdb.com/) http://thetvdb.com/ to get TV series and episodes data.

## License
The MIT License (MIT)  
Copyright (c) 2016 Kacper Zybała  
See: https://github.com/ZyperPL/TVCTL/blob/master/LICENSE  
Also, feel free to contribute!
