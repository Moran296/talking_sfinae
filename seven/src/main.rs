
struct StringStruct {
    string: [u8; 80],
}

impl StringStruct {
    fn new(val: &'static str) -> StringStruct {
        let mut string = [0; 80];
        for (i, c) in val.bytes().enumerate() {
            string[i] = c;
        }

        StringStruct { string }
    }
}



trait ToStr {
    fn c_str<'a>(&'a self) -> &'a str;
}

impl ToStr for StringStruct {
    fn c_str<'a>(&'a self) -> &'a str {
        let mut len = 0;
        for i in 0..80 {
            if self.string[i] == 0 {
                break;
            }
            len += 1;
        }
        unsafe { core::str::from_utf8_unchecked(&self.string[..len]) }
    }
}

fn print_me(s: impl ToStr) {
    println!("{}", s.c_str());
}

fn main() {
    let s = StringStruct::new("Hello World");
    print_me(s);
}
