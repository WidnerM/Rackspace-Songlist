# Rackspace-Songlist
Utility to produce a list of all Rackspaces and each of the Songs/Songparts that utilize it

Once installed, click on Extensions -> Rackspace-Songlist and GP's log window will open.

You then need to cycle through every Song and Songpart so that the extension can build a catalog which Rackspaces are used in which Songs and Songparts.  When it's done it will report them in the log window.  You can copy and paste that into a destination of your choice.  If you want it to be a cleaner copy you should hit "Clear" in the log window before clicking through all your songparts.

For me the easiest way to go through every songpart is to create a button in your global rackspace that links to the System Actions plugin "Next" function.  You'll want to turn off the option "Next/Previous stays within song" on the Options -> Setlist window, and make your "Next" button momentary so you can just spam click the button to advance with every click.

After you've reached the last Songpart of the last Song the extension will print your list.

There will be one line for every rackspace that lists each of the songparts where it is used, formatted as Songname[Variationname]
