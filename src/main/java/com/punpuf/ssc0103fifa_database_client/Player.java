package com.punpuf.ssc0103fifa_database_client;

/**
 * Esta classe representa as características de um jogador FIFA.
 * @author Jean Patrick Ngandu Mamani
 * @author Marcus da Silva Carr
 */
@SuppressWarnings("unused")
public class Player {
    int id;
    int age;
    String nomeJogador;
    String nacionalidade;
    String nomeClube;
    
    public Player(int id, int age, String nomeJogador, String nacionalidade, String nomeClube) {
        this.id = id;
        this.age = age;
        this.nomeJogador = nomeJogador;
        this.nacionalidade = nacionalidade;
        this.nomeClube = nomeClube;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

    public String getNomeJogador() {
        return nomeJogador;
    }

    public void setNomeJogador(String nomeJogador) {
        this.nomeJogador = nomeJogador;
    }

    public String getNacionalidade() {
        return nacionalidade;
    }

    public void setNacionalidade(String nacionalidade) {
        this.nacionalidade = nacionalidade;
    }

    public String getNomeClube() {
        return nomeClube;
    }

    public void setNomeClube(String nomeClube) {
        this.nomeClube = nomeClube;
    }

    @Override
    public String toString() {
        return "Player{" +
                "ID=" + id +
                ", Age=" + age +
                ", Name='" + nomeJogador + '\'' +
                ", Nationality='" + nacionalidade + '\'' +
                ", Club='" + nomeClube + '\'' +
                '}';
    }
}

