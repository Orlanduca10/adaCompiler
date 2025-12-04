procedure Main is
begin
    X := 10;
    while X > 0 loop
        Put_Line("Ola");
        X := X - 1;
        Y:=  X - 3;
        while X > 0 loop
            Put_Line("Countdown");
            X := 10;
        end loop;
    end loop;
end Main;
