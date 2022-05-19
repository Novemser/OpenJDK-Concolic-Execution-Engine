package com;

public class Main {
    public static void main(String...args) throws Exception {
        System.out.println("Hi, com.main!");

        System.weBridgeAnalysis(Class.forName("com.Main").getClassLoader());
    }
}