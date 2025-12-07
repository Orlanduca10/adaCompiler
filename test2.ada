procedure Main is
begin
    Get_Line(X);
    if X = 10 then
        Put_Line(X+10);
    else
        Y := X + 1;
        Put_Line(Y);
    end if;
end Main;