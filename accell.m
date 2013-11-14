function accell()
 
TIMEOUT = 10;    %time to wait for data before aborting
XPOINTS = 500;   %number of points along x axis
 
try
    %create serial object to represent connection to mbed
    mbed = serial('COM4', ...
                  'BaudRate', 115200, ...
                  'Parity', 'none', ...
                  'DataBits', 8, ...
                  'StopBits', 1);       %change depending on mbed configuration
    
    set(mbed,'Timeout',TIMEOUT);        %adjust timeout to ensure fast response when mbed disconnected
    
    fopen(mbed);        %open serial connection
 
    position = 1;       %initialise graph variables
    time = 1;
    x = [(1:XPOINTS)' (1:XPOINTS)' (1:XPOINTS)'];
    xlabels = (1:XPOINTS);
    y = zeros(XPOINTS,3);
    
    while (1)       %loop forever (or until an error occurs)
        
        values = fscanf(mbed, '%f,%f,%f,%f,%f,%f');    %get values into vector
                                              %assumes data formatted as
                                              %'1,2,3'
        y(position,:) = values';    %put into y to be displayed
        
        %update position on x-axis and x-axis labels
        xlabels(position) = time;
        time = time + 1;
        if (position < XPOINTS)
            position = position + 1;
        else
            position = 1;
        end
 
        %display
        plot(x,y);
        set(gca, 'XTick', 1:XPOINTS);
        set(gca, 'XTickLabel', xlabels);
 
        drawnow;    %this is required to force the display to update before the function terminates
    end
 
    fclose(mbed);   %close connection (this should never be reached when using while(1), but included for completeness)
    
catch
    %in case of error or mbed being disconnected
    disp('Failed!');
    fclose(mbed);   %close connection to prevent COM port being lokced open
end 
